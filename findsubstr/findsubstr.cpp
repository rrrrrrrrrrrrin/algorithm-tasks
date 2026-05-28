#define CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <fstream>
#include <iostream>

const int MAX_LEN = 1024;
const int ALPHABET_ASCII = 256;

// ============================ Boyer-Moore (Bm) string-search algorithm
// ============================

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

// Функция, проверяющая что подстрока substr[p...len-1]
// является префиксом шаблона substr
static bool isPrefix(const char* substr, int p, int len) {
  int j = 0;
  for (int i = p; i < len; ++i) {
    if (substr[i] != substr[j]) {
      return false;
    }
    ++j;
  }
  return true;
}

// Функция находит длину макс. суффикса,
// который заканчивается в позиции p
static int suffixLength(const char* substr, int p, int len_substr) {
  int len = 0;
  int i = p;
  int j = len_substr - 1;
  while (i >= 0 && substr[i] == substr[j]) {
    ++len;
    --i;
    --j;
  }
  return len;
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
// Индекс bmGs[i] это длина хорошего суффикса
//
// Gs is good suffixes
static void preBmGs(const char* substr, int len, int bmGs[MAX_LEN + 1]) {
  int lastPrefixPosition = len;

  // Ищет случаи, когда суффикс шаблона является его префиксом
  for (int i = len - 1; i >= 0; --i) {
    // Если substr[i+1...len-1] является префиксом, то запомним её начало
    if (isPrefix(substr, i + 1, len)) {
      lastPrefixPosition = i + 1;
    }
    bmGs[len - 1 - i] = lastPrefixPosition - i + len - 1;
  }

  // Для каждого возможного суффикса ищет, где ещё он встречается в шаблоне,
  // и записывает нужный сдвиг
  for (int i = 0; i < len - 1; ++i) {
    int slen = suffixLength(substr, i, len);
    bmGs[slen] = len - 1 - i + slen;
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
      int shift1 = bmGs[len_substr - 1 - j];  // сдвиг по хорошему суффиксу

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

  // Читаем строку целиком
  while (input.getline(line, MAX_LEN + 1)) {
    int len = static_cast<int>(std::strlen(line));
    cnt += bmCountInLine(line, len, substr, len_substr, bmBc, bmGs);
  }

  std::cout << cnt << '\n';
  input.close();
  return 0;
}
