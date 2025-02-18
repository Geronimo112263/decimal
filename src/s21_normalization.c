#include "s21_decimal.h"

void scale_and_sign_for_arithmetic(s21_decimal value_1, s21_decimal value_2,
                                   s21_decimal *scale_and_sign) {
  null_decimal(scale_and_sign);
  if (value_1.bits[3] == value_2.bits[3]) {
    scale_and_sign->bits[3] = value_1.bits[3];
  } else {
    s21_scale_set(scale_and_sign, s21_scale_get(&value_1));
    int sign_val_1 = s21_sign_get(&value_1);
    int sign_val_2 = s21_sign_get(&value_2);
    value_1.bits[3] = 0;
    value_2.bits[3] = 0;
    if (s21_is_greater(value_1, value_2)) {
      s21_sign_set(scale_and_sign, sign_val_1);
    } else
      s21_sign_set(scale_and_sign, sign_val_2);
  }
}

void scale_and_sign_for_mul(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *scale_and_sign) {
  null_decimal(scale_and_sign);
  int sign_value_1 = s21_sign_get(&value_1);
  int sign_value_2 = s21_sign_get(&value_2);
  if (is_null(&value_1) || is_null(&value_2)) {
    s21_scale_set(scale_and_sign, 0);
  } else if ((sign_value_1 && !sign_value_2) ||
             (!sign_value_1 && sign_value_2)) {
    s21_scale_set(scale_and_sign,
                  s21_scale_get(&value_1) + s21_scale_get(&value_2));
    s21_sign_set(scale_and_sign, 1);
  } else
    s21_scale_set(scale_and_sign,
                  s21_scale_get(&value_1) + s21_scale_get(&value_2));
}

//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//

void big_normalization(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int norm = 0;
  s21_big_decimal *case_value;
  s21_big_decimal step_one;
  s21_big_decimal step_three;
  while ((norm = big_checking_scale(value_1, value_2))) {
    if (norm == 1) {
      case_value = value_2;
    } else
      case_value = value_1;
    big_shift(*case_value, 1, &step_one);
    big_shift(*case_value, 3, &step_three);
    big_add(step_one, step_three, case_value);
    big_scale_add(case_value);
  }
}

// сравнение скейла
int big_checking_scale(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int res = 0;
  int flag_stop = 0;
  int a = 0, b = 0;
  for (int i = 247; i >= 240 && !flag_stop; i--) {
    a = s21_bit_get(value_1->bits, i);
    b = s21_bit_get(value_2->bits, i);
    if (a != b) {
      flag_stop = 1;
      if (a < b) {
        res = -1;
      } else
        res = 1;
    }
  }
  return res;
}

//повышение scale на 1
void big_scale_add(s21_big_decimal *value) {
  int scale = s21_bd_scale_get(value);
  scale++;
  s21_bd_scale_set(value, scale);
}

//сдвиг мантисы на указанное число
void big_shift(s21_big_decimal num, int moving, s21_big_decimal *result) {
  null_big_decimal(result);
  for (int i = 0; i <= 223; i++) {
    s21_set_bit(result->bits, i + moving, s21_bit_get(num.bits, i));
  }
  s21_bd_scale_set(result, s21_bd_scale_get(&num));
  s21_bd_sign_set(result, s21_bd_sign_get(&num));
}