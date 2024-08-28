#include "./../s21_decimal.h"
#include <string.h>


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  // Инициализация статуса и признаков знаков чисел
  int status = 0;
  int valueSign_1 = getSign(value_1);
  int valueSign_2 = getSign(value_2);

  // Обнуление битов результата
  memset(result->bits, 0, sizeof(result->bits));

  // Если знаки у чисел одинаковы
  if (valueSign_1 == valueSign_2) {
    // Выравнивание масштаба
    alignmentScale(&value_1, &value_2);

    // Побитовое сложение
    if (addBit(value_1, value_2, result)) {
      // Проверка на переполнение
      int scale1 = getScale(value_1);
      int scale2 = getScale(value_2);

      if ((!scale1 || !scale2)) {
        status = valueSign_1 ? 2 : 1;  // Переполнение для отрицательных и положительных чисел
      } else {
        // Уменьшение масштаба и рекурсивный вызов сложения
        setScale(decreaseScale(&value_1, 1), scale1 - 1);
        setScale(decreaseScale(&value_2, 1), scale2 - 1);
        status = s21_add(value_1, value_2, result);
      }
    } else {
      // Установка знака и масштаба результата
      setSign(result, valueSign_1);
      setScale(result, getScale(value_1));
    }
  } 
  // Если знаки чисел разные
  else {
    if (valueSign_1) {
      // Если первое число отрицательное, а второе положительное
      status = s21_sub(value_2, *setSign(&value_1, 0), result);
    } else {
      // Если первое число положительное, а второе отрицательное
      status = s21_sub(value_1, *setSign(&value_2, 0), result);
    }
  }

  return status;
}
