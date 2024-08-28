#include "./../s21_decimal.h"

// Функция для проверки, является ли value_1 больше или равно value_2
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // Проверяем, является ли value_1 больше value_2
  // или же равны ли value_1 и value_2
  return s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2);
}