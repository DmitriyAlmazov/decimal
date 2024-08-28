#include "./../s21_decimal.h"

// Функция для умножения двух чисел типа s21_decimal
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Инициализация результата нулями
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 0; // Переменная для отслеживания переполнения или других ошибок
  int sign_result = 5; // Переменная для хранения знака результата

  // Определяем знак результата на основе знаков входных значений
  if (getSign(value_1) != getSign(value_2)) {
    sign_result = 1; // Результат должен быть отрицательным
  } else {
    sign_result = 0; // Результат должен быть положительным
  }

  // Очищаем знаковые биты для процесса умножения
  setSign(&value_1, 0);
  setSign(&value_2, 0);

  // Получаем позицию самого значимого бита в value_1
  int bit1 = getBitLast(value_1);

  // Выполняем умножение с использованием побитовых операций
  for (int i = 0; i <= bit1; i++) {
    s21_decimal tmp_res = {{0, 0, 0, 0}}; // Временная переменная для хранения промежуточных результатов

    // Проверяем, установлен ли i-й бит в value_1
    int value_bit_1 = getBit(value_1, i);

    if (value_bit_1) {
      tmp_res = value_2; // Копируем value_2 во временную переменную tmp_res
      leftShift(&tmp_res, i); // Сдвигаем tmp_res влево на i позиций
      res = addBit(*result, tmp_res, result); // Добавляем tmp_res к результату, учитывая перенос/переполнение
    }
  }

  // Обработка возможного переполнения путем корректировки масштабов
  if (res) {
    while (getScale(value_1) > 0 || getScale(value_2) > 0) {
      s21_decimal *value_1_1, *value_2_1;

      // Определяем, какое значение имеет больший значимый бит и масштаб
      if (getBitLast(value_1) > getBitLast(value_2) && getScale(value_1) > 0) {
        value_1_1 = &value_1;
        value_2_1 = &value_2;
      } else if (getBitLast(value_2) > getBitLast(value_1) && getScale(value_2) > 0) {
        value_1_1 = &value_2;
        value_2_1 = &value_1;
      } else {
        break; // Выходим из цикла, если ни одно из значений не может быть дополнительно скорректировано
      }

      // Уменьшаем масштаб более значимого числа для обработки переполнения
      int scale2 = getScale(*value_1_1);
      decreaseScale(value_1_1, 1);
      setScale(value_1_1, --scale2);

      // Повторяем умножение с корректированными значениями
      return s21_mul(*value_1_1, *value_2_1, result);
    }
  }

  // Вычисляем и устанавливаем масштаб результата на основе масштабов входных значений
  int scale = getScale(value_1) + getScale(value_2);
  setScale(result, scale);

  // Устанавливаем знак результата
  setSign(result, sign_result);

  // Если было переполнение и результат отрицательный, корректируем код ошибки
  if (res && sign_result) res = 2;

  return res; // Возвращаем статус, указывающий на успех или тип ошибки (например, переполнение)
}
