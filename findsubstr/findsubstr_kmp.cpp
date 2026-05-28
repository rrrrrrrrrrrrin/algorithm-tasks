#include <cstring>
#include <fstream>
#include <iostream>

const int MAX_LEN = 1024;
const size_t BUFFER_SIZE = 1048576;  // Читаем блоками по 1 МБ

// Knuth–Morris–Pratt algorithm

// Построение префикс-функции
//
// pl[i] - длина наибольшего собственного префикса подстроки substr[0..i],
// который одновременно является ее суффиксом
static void buildPrefixFunction(const char* substr, int m, int pl[]) {
  pl[0] = 0;
  int j = 0;  // длина текущего совпавшего префикса

  // i это текущая позиция в шаблоне
  // pl[0] = 0 (у одного символа нет собственного префикса), поэтому начинаем с
  // i = 1
  for (int i = 1; i < m; ++i) {
    // Пока символы не совпадают
    while (j > 0 && substr[i] != substr[j]) {
      // Несовпадение; не начинаем заново
      // Уменьшаем текущий совпавший префикс
      // до возможного подходящего префикса
      //
      // Пр: substr - ABABAC, допустим ABABA уже совпало => j = 5
      //     C несовпало
      //     Нет ли внутри уже совпавшей части меньшего префикса?
      //     Есть префикс-суффикс - ABA, его длина 3
      //     => j = pl[5 - 1 = 4] = 3
      //     Продолжаем цикл с j = 3, ABA
      j = pl[j - 1];
    }
    // Все символы совпали
    if (substr[i] == substr[j]) {
      ++j;  // увеличиваем длину совпавшего префикса
    }
    pl[i] = j;  // запоминаем длину совпавшего префикса для позиции i
  }
}

// // Подсчет вхождений substr в line
// static int kmpCountInLine(const char* line, int n, const char* substr, int m,
//                           const int pl[]) {
//   // Искомая подстрока пустая или длиннее строки
//   if (m == 0 || n < m) {
//     return 0;
//   }

//   // Поиск подстроки
//   int count = 0;
//   int j = 0;  // сколько символов шаблона уже совпало

//   // i идет по line слева направо
//   for (int i = 0; i < n; ++i) {
//     // Если текущий символ текста не совпал с символом шаблона,
//     // мы не начинаем с нуля, а переходим к более короткому подходящему
//     префиксу while (j > 0 && line[i] != substr[j]) {
//       j = pl[j - 1];
//     }

//     // Если текущий символ текста совпал с нужным символом шаблона,
//     // увеличиваем длину совпадения
//     if (line[i] == substr[j]) {
//       ++j;
//     }

//     // Если совпал весь шаблон
//     if (j == m) {
//       ++count;

//       // Продолжим поиск, не пропуская перекрывающиеся вхождения substr
//       j = pl[j - 1];
//     }
//   }

//   return count;
// }

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: substr input-file\n";
    return 1;
  }

  // Копируем шаблон и вычисляем длину за один проход
  char substr[MAX_LEN + 1];
  int len_substr = 0;
  while (len_substr < MAX_LEN && argv[1][len_substr] != '\0') {
    substr[len_substr] = argv[1][len_substr];
    len_substr++;
  }
  substr[len_substr] = '\0';

  // Подстрока пустая
  if (len_substr == 0) {
    std::cout << 0 << '\n';
    return 0;
  }

  FILE* input = fopen(argv[2], "rb");
  if (input == nullptr) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  // Строим префикс-функцию (зависит только от substr,
  // т.е. строим ее только один раз до чтения файла)
  int pl[MAX_LEN];
  buildPrefixFunction(substr, len_substr, pl);

  char buffer[BUFFER_SIZE];

  int cnt = 0;  // Подсчет вхождений substr в line
  int j = 0;    // сколько символов шаблона уже совпало

  int bytesRead;

  // Читаем файл кусками
  while ((bytesRead = static_cast<int>(fread(buffer, 1, BUFFER_SIZE, input))) >
         0) {
    // Прогоняем КМП прямо по сырому буферу
    // i идет по line слева направо
    for (int i = 0; i < bytesRead; ++i) {
      // Имитируем поведение kmpCountInLine:
      // сбрасываем поиск при переходе на новую строку
      if (buffer[i] == '\n') {
        j = 0;
        continue;
      }

      // Если текущий символ текста не совпал с символом шаблона,
      // мы не начинаем с нуля, а переходим к более короткому подходящему
      // префиксу
      while (j > 0 && buffer[i] != substr[j]) {
        j = pl[j - 1];
      }

      // Если текущий символ текста совпал с нужным символом шаблона,
      // увеличиваем длину совпадения
      if (buffer[i] == substr[j]) {
        ++j;
      }

      // Если совпал весь шаблон
      if (j == len_substr) {
        ++cnt;

        // Продолжим поиск, не пропуская перекрывающиеся вхождения substr
        j = pl[j - 1];
      }
    }
  }

  std::cout << cnt << '\n';
  fclose(input);
  return 0;
}
