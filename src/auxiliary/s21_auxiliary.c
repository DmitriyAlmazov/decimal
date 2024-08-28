#include "./../s21_decimal.h"

// Функция для получения значения конкретного бита в числе
int getBit(s21_decimal value, int bit) {
  // В этой строке вычисляется значение конкретного бита:
  // - Берем соответствующий 32-битный блок числа (value.bits[bit / 32]),
  // - Сдвигаем единицу на нужное количество позиций (1u << (bit % 32)),
  // - Выполняем побитовую операцию И (&), чтобы получить значение бита.
  // Пример:
  // Если bit = 5, то 1 << (5 % 32) = 0b100000.
  return !!(value.bits[bit / 32] & (1u << (bit % 32)));
}

// Функция для нахождения последнего установленного бита
int getBitLast(s21_decimal value) {
  int bitLast = 95; // Максимальный бит для числа s21_decimal (от 0 до 95)
  
  // Проходим в цикле по битам с конца, пока не найдем установленный бит (единицу).
  // Например, если число равно 10100000..., то первый найденный установленный бит будет на позиции 3.
  for (; (bitLast >= 0) && (!getBit(value, bitLast));) bitLast -= 1;

  return bitLast; // Возвращаем индекс последнего установленного бита
}

// Функция для установки конкретного бита в заданную позицию
s21_decimal *setBit(s21_decimal *value, int pos, int bit) {
  // Проверяем, что позиция находится в допустимом диапазоне (0-95)
  if (pos / 32 < 4 && bit)
    // Устанавливаем бит: сдвигаем единицу на нужное количество позиций и выполняем побитовую операцию ИЛИ.
    // Пример: если pos = 5, то 1u << (5 % 32) = 0b100000, и это устанавливает бит на пятую позицию.
    value->bits[pos / 32] |= (1u << (pos % 32));
  else if (pos / 32 < 4 && !bit)
    // Если бит нужно сбросить, используем побитовую операцию И с отрицанием, чтобы обнулить нужный бит.
    value->bits[pos / 32] &= ~(1u << (pos % 32));
  
  return value;
}


int addBit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int fres = 0, exp = 0; // fres — флаг переполнения, exp — перенос при сложении битов

  // Проходим по всем битам (0-95)
  for (int i = 0; i < 96; i += 1) {
    // Получаем значения битов для двух чисел на текущей позиции
    int bitValue_1 = getBit(value_1, i), bitValue_2 = getBit(value_2, i);

    // Логика сложения битов
    if (!bitValue_1 && !bitValue_2) {
      // Если оба бита равны 0, то результат зависит только от наличия переноса (exp).
      // Если перенос есть (exp == 1), то устанавливаем бит в результат и сбрасываем перенос.
      exp ? setBit(result, i, (exp = 0) + 1) : setBit(result, i, 0);
    } else if (bitValue_1 != bitValue_2) {
      // Если один из битов равен 1, а другой — 0, то результат зависит от переноса.
      // Если перенос был (exp == 1), то результат будет 0, но перенос сохранится.
      exp ? setBit(result, i, (exp = 1) - 1) : setBit(result, i, 1);
    } else {
      // Если оба бита равны 1, то устанавливаем перенос (exp = 1) и в зависимости от него результат может быть 0.
      exp ? setBit(result, i, (exp = 1)) : setBit(result, i, (exp = 1) - 1);
    }

    // Проверка на переполнение: если последний бит установлен и есть перенос, устанавливаем флаг fres.
    if (i == 95 && exp == 1 && !getBit(value_1, 97) && !getBit(value_2, 97))
      fres = 1;
  }

  return fres; // Возвращаем флаг переполнения
}

// Функция для получения масштаба (scale) десятичного числа
int getScale(s21_decimal value) {
  // Масштаб хранится в битах с 16-го по 23-й (8 бит) в четвертом 32-битном блоке (value.bits[3]).
  int result = (char)(value.bits[3] >> 16); // Сдвигаем на 16 бит вправо, чтобы получить значение масштаба.
  return result;
}

// Функция для установки масштаба (scale)
s21_decimal *setScale(s21_decimal *value, int scale) {
  if (scale >= 0 && scale <= 28) { // Масштаб может находиться в пределах от 0 до 28
    int sign = getSign(*value); // Получаем текущий знак числа
    
    // Обнуляем старое значение масштаба: сбрасываем биты с 16-го по 23-й
    value->bits[3] &= ~(0xFF << 16);
    
    // Устанавливаем новое значение масштаба, сдвигая его на 16 бит влево
    value->bits[3] |= scale << 16;
    
    // Восстанавливаем знак числа, если он был отрицательным
    sign ? setSign(value, 1) : value;
  }
  return value;
}

// Функция для увеличения масштаба числа
s21_decimal *increaseScale(s21_decimal *value, int shift) {
  // Проверяем, что масштаб не выходит за допустимые пределы
  if (getScale(*value) + shift < 29) {
    setScale(value, getScale(*value) + shift); // Увеличиваем масштаб
    
    // Копируем исходное значение в два временных переменных
    s21_decimal auxValue_1 = *value, auxValue_2 = *value;
    
    // Сдвигаем оба числа влево и суммируем их. Если возникает переполнение, устанавливаем бит 96.
    if (addBit(*leftShift(&auxValue_1, 1), *leftShift(&auxValue_2, 3), value))
      setBit(value, 96, 1); // Устанавливаем бит для индикации переполнения
  }
  return value;
}


s21_decimal *decreaseScale(s21_decimal *value, int shift) {
  // Цикл для уменьшения масштаба на указанное количество сдвигов
  for (int y = 0; y < shift; y += 1) {
    unsigned long long overflow = value->bits[2]; // Переполнение
    
    // Проходим по блокам числа и делим его на 10
    for (int x = 2; x >= 0; x -= 1) {
      value->bits[x] = overflow / 10; // Делим блок на 10
      overflow =
          (overflow % 10) * (S21_MAX_UINT + 1) + value->bits[x ? x - 1 : x]; // Переносим остаток
    }
  }

  // Устанавливаем новый масштаб
  setScale(value, (getScale(*value) - shift));
  return value;
}

// Функция для выравнивания масштаба двух чисел
void alignmentScale(s21_decimal *value_1, s21_decimal *value_2) {
  if (getScale(*value_1) != getScale(*value_2)) {
    if (getScale(*value_1) < getScale(*value_2)) {
      // Если у второго числа масштаб больше, меняем местами числа и повторяем
      alignmentScale(value_2, value_1);
    } else {
      int scaleLow = getScale(*value_2), scaleHigh = getScale(*value_1);
      
      // Увеличиваем масштаб второго числа до выравнивания с первым числом
      for (; (scaleHigh - scaleLow) && !getBit(*value_2, 96); scaleLow += 1)
        increaseScale(value_2, 1);

      // Уменьшаем масштаб первого числа до выравнивания с вторым числом
      for (; scaleHigh - scaleLow; scaleHigh -= 1) {
        decreaseScale(value_1, scaleHigh - scaleLow);
        setScale(value_1, scaleLow);
      }
    }
  }
}

// Функция для получения знака числа (0 — положительное, 1 — отрицательное)
int getSign(s21_decimal value) {
  return !!(value.bits[3] & (1u << 31)); // Проверяем 31-й бит четвертого блока
}

// Функция для установки знака числа
s21_decimal *setSign(s21_decimal *value, int bit) {
  // Если bit == 1, устанавливаем знак (отрицательное число), если 0 — сбрасываем
  value->bits[3] = (bit) ? (value->bits[3] | (1u << 31)) : (value->bits[3] & ~(1u << 31));
  return value;
}

// Функция для конвертации числа (обращение знака)
s21_decimal *convert(s21_decimal *value) {
  s21_decimal result = {{0, 0, 0, 0}}; // Результат
  s21_decimal add = {{1, 0, 0, 0}};    // Единичное значение для сложения

  // Инвертируем биты числа
  for (int x = 0; x < 3; x += 1) value->bits[x] = ~value->bits[x];

  // Выполняем сложение числа с единицей
  addBit(*value, add, &result);

  // Присваиваем результат оригинальному числу
  for (int x = 0; x < 3; x += 1) value->bits[x] = result.bits[x];

  // Устанавливаем бит, чтобы указать, что число отрицательное
  setBit(value, 97, 1);
  
  return value;
}

// Функция для проверки, является ли число нулевым
int isNull(s21_decimal value) {
  return !value.bits[0] && !value.bits[1] && !value.bits[2]; // Проверяем, что все три блока равны нулю
}

// Функция для получения порядка экспоненты числа с плавающей запятой
int getFloatExp(float *value) {
  // Извлекаем порядок из числа с плавающей запятой
  return ((*((int *)value) & ~(1u << 31)) >> 23) - 127; // Извлекаем биты экспоненты и вычитаем смещение
}

// Функция для сдвига десятичного числа влево на указанное количество бит
s21_decimal *leftShift(s21_decimal *value, int shift) {
  // Проверяем, не выходим ли за пределы допустимых битов
  if (!(getBitLast(*value) + shift > 95)) {
    for (int y = 0; y < shift; y += 1) {
      // Получаем значения битов на границе блоков для переноса
      int bitTransfer_1 = getBit(*value, 31); // Получаем бит на 31-й позиции
      int bitTransfer_2 = getBit(*value, 63); // Получаем бит на 63-й позиции

      // Сдвигаем каждый блок числа влево на один бит
      for (int x = 0; x < 3; x += 1) value->bits[x] <<= 1;

      // Устанавливаем перенесенные биты в соседние блоки
      if (bitTransfer_1) setBit(value, 32, 1); // Устанавливаем бит на 32-й позиции
      if (bitTransfer_2) setBit(value, 64, 1); // Устанавливаем бит на 64-й позиции
    }
  }
  return value;
}
