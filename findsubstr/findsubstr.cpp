#define CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <fstream>
#include <iostream>

const int MAX_LEN = 1024;
const int ALPHABET_ASCII = 256;

// Boyer-Moore (Bm) string-search algorithm

// Функция для вычисления таблицы сдвигов плохих символов
//
// Для каждого символа таблица говорит, на сколько сдвинуть шаблон,
// если этот символ вызвал несовпадение
//
// Она будет равна длине шаблона для всех символов, которые
// не встречаются в шаблоне (и последнего символа шаблона),
// и порядковому номеру с конца для остальных
//
// Bc is bad chars
static void preBmBc(const char* substr, int len, int bmBc[ALPHABET_ASCII]) {
  for (int i = 0; i < ALPHABET_ASCII; ++i) {
    bmBc[i] = len;
  }
  for (int i = 0; i < len - 1; ++i) {
    bmBc[(unsigned char)substr[i]] =
        len - 1 - i;  // unsigned char to wrap around ALPHABET 0-255
  }
}

// Таблица хороших суффиксов
//
// Если часть шаблона справа совпала,
// а потом случилось несовпадение,
// эта таблица указывает на то, как далеко можно сдвинуть шаблон, чтобы
// совпавший суффикс совпал с префиксом шаблона
// или
// совпавший суффикс встретился ещё раз внутри шаблона
//
// Индекс bmGs[i] это смещение (на сколько символов сдвинуть шаблон),
// которое нужно сделать, если несовпадение произошло в позиции j (считая с
// конца шаблона) i это длина суффикса, который мы уже успешно сопоставили в
// тексте перед тем, как произошло несовпадение
//
// Gs is good suffixes
//
// Gusfield algorithm
static void preBmGs(const char* substr, int len, int bmGs[MAX_LEN + 1]) {
  // Длина наибольшего общего суффикса подстроки, заканчивающейся в позиции i;
  // и длина всего шаблона; в suff
  int suff[MAX_LEN + 1];

  suff[len - 1] = len;
  int g = len - 1;
  int f = len - 1;

  // Проходим по шаблону справа налево
  for (int i = len - 2; i >= 0; --i) {
    // Проверка по памяти
    // Если текущий индекс i находится внутри уже найденного отрезка [g, f],
    // мы можем использовать уже вычисленные значения в suff для оптимизации
    //
    // i + len - 1 - f это соответствующая позиция в уже обработанной части
    if (i > g && suff[i + len - 1 - f] < i - g) {
      suff[i] = suff[i + len - 1 - f];  // Просто копируем результат
    } else {
      // Пересчет g и f для нового отрезка [g, i]
      //
      // Если мы вышли за границы окна или предыдущие данные не подходят,
      // начинаем расширять границы окна от текущей позиции
      if (i < g) {
        g = i;  // Сдвигаем левую границу окна
      }

      f = i;  // Устанавливаем новую правую границу окна на текущем индексе

      // Сравниваем символы шаблона, двигаясь влево от текущей границы f,
      // пока символы совпадают или мы не дошли до начала шаблона
      while (g >= 0 && substr[g] == substr[g + len - 1 - f]) {
        --g;
      }

      // Записываем длину совпавшего суффикса f - g
      suff[i] = f - g;
    }
  }

  // Заполнение базовыми сдвигами
  for (int i = 0; i < len; ++i) {
    bmGs[i] = len;
  }

  // Обработка случая, когда суффикс является префиксом шаблона
  //
  // Если суффикс шаблона совпадает с его префиксом, то при несовпадении
  // можно сдвинуть шаблон так, чтобы этот суффикс совпал с префиксом
  for (int i = len - 1; i >= 0; --i) {
    if (suff[i] == i + 1) {
      for (int j = 0; j < len - 1 - i; ++j) {
        if (bmGs[j] == len) {
          bmGs[j] = len - 1 - i;
        }
      }
    }
  }

  // Обработка случая, когда суффикс встречается внутри шаблона
  //
  // Если суффикс шаблона встречается внутри шаблона, то при несовпадении
  // можно сдвинуть шаблон так, чтобы этот суффикс совпал с
  // другой копией того же суффикса, найденной ранее
  for (int i = 0; i <= len - 2; ++i) {
    bmGs[len - 1 - suff[i]] = len - 1 - i;
  }
}

// Подсчет вхождений substr в line
static int bmCountInLine(const char* line, int len, const char* substr,
                         int len_substr, const int bmBc[ALPHABET_ASCII],
                         const int bmGs[MAX_LEN + 1]) {
  // Искомая подстрока пустая или длиннее строки
  if (len_substr == 0 || len < len_substr) {
    return 0;
  }

  // Поиск подстроки

  int count = 0;

  // Сдвиг substr относительно line
  int i = 0;

  // Сравнение справа налево по line
  while (i <= len - len_substr) {
    int j = len_substr - 1;

    // Сравнение справа налево по substr
    while (j >= 0 && substr[j] == line[i + j]) {
      // Если символы совпадают, то двигаем указатель в substr влево
      --j;
    }

    // Если совпали все символы (j < 0),
    // шаблон substr найден в line
    if (j < 0) {
      ++count;

      // Чтобы не пропустить перекрывающиеся вхождения подстроки в строку
      // Пр: line - |ABAB|A, substr - ABAB
      //     у substr совпали префикс и суффикс AB
      //     bmGs[0] = 2  (длина хорошего суффикса 0, т.к. совпала вся substr)
      //     line - |ABAB|A -> AB|ABA|
      i += bmGs[0];
    } else {  // было несовпадение
      // Делаем макс. сдвиг
      int shift1 = bmGs[j];  // сдвиг по хорошему суффиксу

      int shift2 = bmBc[(unsigned char)line[i + j]] -
                   (len_substr - 1 - j);  // сдвиг по плохому символу
      if (shift2 < 1) {
        shift2 = 1;  // сдвиг по плохому символу должен быть хотя бы 1
      }

      i += (shift1 > shift2) ? shift1 : shift2;
    }
  }

  return count;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: substr input-file\n";
    return 1;
  }

  char substr[MAX_LEN + 1];
  std::strncpy(substr, argv[1], MAX_LEN + 1);
  substr[MAX_LEN] = '\0';
  int len_substr = static_cast<int>(std::strlen(substr));

  std::ifstream input(argv[2]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  // Подготовка таблиц (зависит только от substr,
  // (т.е. строим таблицы один раз до чтения файла)
  int bmBc[ALPHABET_ASCII];  // таблица плохих символов
  int bmGs[MAX_LEN + 1];     // таблица хороших суффиксов

  preBmBc(substr, len_substr, bmBc);
  preBmGs(substr, len_substr, bmGs);

  char line[MAX_LEN + 1];
  int cnt = 0;

  while (true) {
    input.getline(line, MAX_LEN + 1);

    if (input.eof() && input.gcount() == 0) {
      break;
    }

    if (input.fail()) {
      input.clear();
    }

    int len = static_cast<int>(std::strlen(line));

    // If file uses Windows line endings (\r\n), getline only removes \n
    // Delete \r
    if (len > 0 && line[len - 1] == '\r') {
      line[len - 1] = '\0';
      len--;
    }

    cnt += bmCountInLine(line, len, substr, len_substr, bmBc, bmGs);
  }

  std::cout << cnt << '\n';
  input.close();
  return 0;
}
