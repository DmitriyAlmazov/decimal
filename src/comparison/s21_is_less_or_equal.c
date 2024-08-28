#include "./../s21_decimal.h" // Подключаем заголовочный файл, который содержит описание структуры s21_decimal и декларации других функций

// Функция для проверки, является ли число value_1 больше или равно числу value_2
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // Возвращаем результат логической операции: true (1), если одно из условий выполняется
  // Проверка условия "больше" с помощью функции s21_is_greater(value_1, value_2)
  // Если value_1 больше value_2, то функция возвращает 1 (true).
  // Например, если value_1 = 10, а value_2 = 5, то s21_is_greater вернет 1.
  
  // Логическая операция || означает "или". Если первое условие выполняется, второе не проверяется.
  // Если первое условие (s21_is_greater) не выполняется, проверяем равенство чисел с помощью s21_is_equal(value_1, value_2).
  // Например, если value_1 = 5 и value_2 = 5, s21_is_equal вернет 1 (true).

  return s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2);
}