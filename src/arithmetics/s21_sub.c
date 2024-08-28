#include "./../s21_decimal.h"

// Функция для вычитания двух чисел типа s21_decimal
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = 0; // Переменная для отслеживания статуса операции (например, переполнение)
  int valueSign_1 = getSign(value_1); // Получаем знак первого числа
  int valueSign_2 = getSign(value_2); // Получаем знак второго числа

  // Инициализируем результат нулями
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;

  // Если знаки чисел разные, то вычитание сводится к сложению
  if (valueSign_1 != valueSign_2) {
    // Приводим оба числа к положительным и складываем их
    status = s21_add(*setSign(&value_1, 0), *setSign(&value_2, 0), result);
    
    // Если произошло переполнение, корректируем статус в зависимости от знака первого числа
    status = status ? (valueSign_1 ? 2 : 1) : status;
    
    // Устанавливаем знак результата в соответствии со знаком первого числа
    setSign(result, valueSign_1);
  } else {
    // Если знаки одинаковые, выравниваем масштабы перед вычитанием
    alignmentScale(&value_1, &value_2);

    // Если числа не равны, выполняем вычитание
    if (!s21_is_equal(value_1, value_2)) {
      // Убираем знаки для корректного выполнения операций
      setSign(&value_1, 0);
      setSign(&value_2, 0);

      // Если первое число меньше второго
      if (s21_is_less(value_1, value_2)) {
        // Вычитаем из второго числа первое и устанавливаем отрицательный знак
        status = s21_add(*convert(&value_1), value_2, result);
        setSign(result, !valueSign_2);
      } else {
        // Вычитаем из первого числа второе и сохраняем знак первого числа
        status = s21_add(*convert(&value_2), value_1, result);
        setSign(result, valueSign_1);
      }
    }
  }
  
  return status; // Возвращаем статус операции
}