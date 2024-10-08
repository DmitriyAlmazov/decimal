#include "./../s21_decimal.h" // Подключаем заголовочный файл с определением структуры s21_decimal и вспомогательных функций.

// Улучшенная версия функции s21_from_int_to_decimal, которая преобразует целое число в десятичный формат s21_decimal.
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = 1; // Инициализируем результат как неуспешный (1 означает ошибку).

  // Проверка, что указатель на результат (dst) не равен NULL.
  if (dst != NULL) { 
    // Инициализация всех битов структуры dst в ноль, чтобы очистить любые предшествующие данные.
    dst->bits[0] = 0; 
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0;

    // Определяем знак числа.
    if (src < 0) { // Если входное целое число отрицательное,
      setSign(dst, 1); // устанавливаем бит знака в структуре dst в 1 (означает отрицательное число).
      src = -src; // Приводим отрицательное число к положительному для дальнейшей обработки.
    }

    // Записываем абсолютное значение целого числа в первый элемент массива bits.
    dst->bits[0] = src; // Целое число помещается в первые 32 бита структуры dst.

    result = 0; // Устанавливаем результат как успешный (0 означает успех).
  }

  return result; // Возвращаем результат: 0 - успех, 1 - ошибка.
}
