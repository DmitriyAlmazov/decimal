#include "./../s21_decimal.h" // Подключаем заголовочный файл с описанием структуры s21_decimal и других необходимых функций
#include <math.h> // Подключаем библиотеку math.h для использования математических функций, таких как pow и fmod

// Улучшенная версия функции s21_from_float_to_decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 1; // Инициализируем результат как неуспешный. Вернем 1, если функция завершится ошибкой.
  
  int exp = getFloatExp(&src); // Получаем экспоненту числа с помощью вспомогательной функции getFloatExp.
  
  // Проверка на валидность входных данных
  if (dst && src != 0 && !isinf(src)) { // Убедимся, что указатель на результат не равен NULL, число не равно нулю и не является бесконечностью.
    
    // Инициализация всех битов dst в ноль
    dst->bits[0] = 0; 
    dst->bits[1] = 0; 
    dst->bits[2] = 0; 
    dst->bits[3] = 0; 

    int sign = (src < 0) ? 1 : 0; // Определяем знак числа: если число отрицательное, sign = 1, иначе sign = 0.
    if (sign) {
      src = -src; // Если число отрицательное, делаем его положительным для дальнейшей обработки.
    }

    double tmp = (double)src; // Промежуточное значение для вычислений. Переводим float в double для повышения точности.
    int scale = 0; // Масштаб (scale) определяет, сколько знаков после запятой требуется для представления числа.

    // Нормализуем число, чтобы оно стало целым
    while (scale < 28 && (int)tmp / (int)pow(2, 21) == 0) {
      tmp *= 10; // Умножаем число на 10, чтобы избавиться от дробной части.
      scale++;   // Увеличиваем масштаб.
    }

    // Проверяем корректность масштаба и экспоненты
    if (scale <= 28 && (exp > -94 && exp < 96)) {
      // Если масштаб в допустимых пределах (0 <= scale <= 28) и экспонента в разумных пределах, продолжаем.
      
      fbits mant; // Вспомогательная структура для работы с мантиссой числа (перевод в побитовую структуру).
      tmp = (float)tmp; // Преобразуем промежуточное значение обратно в float для дальнейших операций.

      // Убираем лишние нули в конце числа, чтобы сохранить только значимые цифры.
      while (fmod(tmp, 10) == 0 && scale > 0) {
        scale--; // Уменьшаем масштаб, если находим нули.
        tmp /= 10; // Делим на 10 для удаления нулей.
      }

      mant.fl = tmp; // Записываем нормализованное значение во вспомогательную структуру для дальнейшего разбора на биты.
      exp = getFloatExp(&mant.fl); // Снова получаем экспоненту нормализованного числа.

      // Устанавливаем биты мантиссы числа
      setBit(dst, exp, 1); // Устанавливаем бит экспоненты в числе dst.
      
      // Разбираем мантиссу побитово и устанавливаем соответствующие биты в dst
      for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
        if ((mant.ui & (1 << j)) != 0) { // Проверяем, установлен ли бит мантиссы на позиции j.
          setBit(dst, i, 1); // Устанавливаем бит в dst на соответствующую позицию i.
        }
      }

      setSign(dst, sign); // Устанавливаем знак числа в dst.
      setScale(dst, scale); // Устанавливаем масштаб (количество знаков после запятой) в dst.
      
      res = 0; // Успешное выполнение, возвращаем 0.
    }
  }

  return res; // Возвращаем результат: 0 в случае успеха, 1 в случае ошибки.
}
