#include "./../s21_decimal.h" // Подключение заголовочного файла s21_decimal.h, содержащего определения и функции для работы с типом s21_decimal

// Функция для вычисления отрицательного значения s21_decimal
// Принимает значение s21_decimal (value) и указатель на результат (result)
// Возвращает 0 в случае успешного выполнения
int s21_negate(s21_decimal value, s21_decimal* result) {
  // Копирование значения с изменением знака
  // Если знак значения value равен 1 (отрицательное число), то устанавливаем положительный знак (0)
  // Иначе устанавливаем отрицательный знак (1)
  // Функция s21_reset обнуляет результат перед копированием
  // Функция s21_copy копирует значение с измененным знаком в результат
  s21_copy(
    s21_reset(result), // Обнуление результата и передача в s21_copy
    getSign(value) ? *setSign(&value, 0) : *setSign(&value, 1) // Если значение отрицательное (getSign(value) возвращает 1), устанавливаем положительный знак, иначе устанавливаем отрицательный
  );
  
  // Возврат 0, чтобы указать успешное выполнение функции
  return 0;
}
