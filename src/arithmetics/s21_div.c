#include "./../s21_decimal.h"
#include <string.h>

// Вспомогательная функция для выполнения деления
int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, s21_decimal *remainder);

// Основная функция деления s21_div
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    // Инициализация результата
    for (int i = 0; i < 4; i++) {
        result->bits[i] = 0;
    }

    int status = 0;
    int sign = 0;

    // Получаем знаки чисел
    int sign1 = getSign(value_1);
    int sign2 = getSign(value_2);

    // Определение знака результата
    if (sign1 != sign2) {
        sign = 1;
    }

    // Устанавливаем оба числа как положительные для дальнейших расчетов
    setSign(&value_1, 0);
    setSign(&value_2, 0);

    // Проверка деления на ноль
    if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
        status = 3;  // Деление на ноль
    } else {
        s21_decimal remainder = {0};

        // Сбрасываем масштабы для целого деления
        setScale(&value_1, 0);
        setScale(&value_2, 0);

        // Выполняем деление с помощью вспомогательной функции
        s21_div1(value_1, value_2, result, &remainder);
        status = 0;
    }

    // Устанавливаем правильный знак результата
    setSign(result, sign);

    return status;
}

// Вспомогательная функция для побитового деления
int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, s21_decimal *remainder) {
    // Проход по битам value_1 от старшего к младшему
    for (int i = getBitLast(value_1); i >= 0; i--) {
        // Сдвигаем временное значение вправо и добавляем текущий бит
        leftShift(remainder, 1);
        if (getBit(value_1, i)) {
            setBit(remainder, 0, 1);
        }

        // Проверяем, если временное значение больше или равно делителю
        if (s21_is_greater_or_equal(*remainder, value_2)) {
            // Вычитаем делитель и сдвигаем результат влево
            s21_sub(*remainder, value_2, remainder);
            leftShift(result, 1);
            setBit(result, 0, 1);  // Устанавливаем бит результата в 1
        } else {
            leftShift(result, 1);  // Сдвиг результата влево, если меньше
        }
    }

    return 0;  // Возвращаем успешный результат
}

