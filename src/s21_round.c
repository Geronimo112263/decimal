#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  null_decimal(result);
  int sign = s21_sign_get(&value);
  int scale = s21_scale_get(&value);
  if (sign) {
    s21_sign_set(&value, false);
  }
  s21_big_decimal big_value = s21_convert_to_big(value);
  big_value.bits[7] = 0;
  int error = 0;
  s21_big_decimal add_rounding = {{1, 0, 0, 0, 0, 0, 0, 0}};
  s21_decimal compar = {{5, 0, 0, 0}};
  s21_big_decimal residual = {0};
  if (scale <= 28) {
    while (scale) {
      residual = s21_div_by_ten_without_rem(&big_value);
      if (s21_is_greater_or_equal(s21_convert_to_small(residual), compar) &&
          scale == 1) {
        big_add(big_value, add_rounding, &big_value);
      }
      scale--;
    }
  } else
    error = 1;
  if (error == 0) {
    big_value.bits[7] = 0;
    *result = s21_convert_to_small(big_value);
    s21_sign_set(result, sign);
  }
  return error;
}