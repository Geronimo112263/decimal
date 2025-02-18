#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  null_decimal(result);
  int error = 0;
  int sign = s21_sign_get(&value);
  int scale = s21_scale_get(&value);
  if (sign) {
    s21_sign_set(&value, false);
  }
  s21_big_decimal big_value = s21_convert_to_big(value);
  if (scale <= 28) {
    while (scale) {
      s21_convert_to_small(s21_div_by_ten_without_rem(&big_value));
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