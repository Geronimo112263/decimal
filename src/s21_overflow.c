#include "s21_decimal.h"

int overflow(s21_big_decimal *value) {
  int error = 0;
  int sign = s21_bd_sign_get(value);
  int scale = s21_bd_scale_get(value);
  if (is_null_big_decimal(value)) {
    value->bits[7] = 0;
  }
  while (checking_mantise(*value) && scale) {
    value->bits[7] = 0;
    ochko_banka(value);
    scale--;
    s21_bd_scale_set(value, scale);
    s21_bd_sign_set(value, sign);
  }
  if (checking_mantise(*value) && !scale) {
    if (sign) {
      error = 2;
    } else
      error = 1;
  }
  if (s21_bd_scale_get(value) > 28 && !checking_mantise(*value)) {
    if (sign) {
      error = 2;
    } else
      error = 1;
  }
  s21_bd_scale_set(value, scale);
  return error;
}

int checking_mantise(s21_big_decimal value) {
  int error = 0;
  if (value.bits[3] || value.bits[4] || value.bits[5] || value.bits[6]) {
    error = 1;
  }
  return error;
}

void ochko_banka(s21_big_decimal *result) {
  s21_big_decimal compar = {{5, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal add_rounding = {{1, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal residual = {0};
  residual = s21_div_by_ten_without_rem(result);
  if (s21_bit_get(result->bits, 0) &&
      s21_is_greater_or_equal(s21_convert_to_small(residual),
                              s21_convert_to_small(compar))) {
    big_add(*result, add_rounding, result);
  } else if (!s21_bit_get(result->bits, 0) &&
             s21_is_greater(s21_convert_to_small(residual),
                            s21_convert_to_small(compar))) {
    big_add(*result, add_rounding, result);
  }
}
