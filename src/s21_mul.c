#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  null_decimal(result);
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  s21_big_decimal big_result = {0};
  big_mul(big_value_1, big_value_2, &big_result);
  int error = overflow(&big_result);
  if (!error) {
    *result = s21_convert_to_small(big_result);
  }
  return error;
}

int big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *result) {
  int error_mul = 0;
  null_big_decimal(result);
  s21_big_decimal value_shift = {0};
  s21_decimal scale_and_sign = {0};
  scale_and_sign_for_mul(s21_convert_to_small(value_1),
                         s21_convert_to_small(value_2), &scale_and_sign);
  value_1.bits[7] = 0;
  value_2.bits[7] = 0;
  for (int i = 0; i <= 223; i++) {
    if (s21_bit_get(value_2.bits, i)) {
      big_shift(value_1, i, &value_shift);
      big_add(*result, value_shift, result);
    }
  }
  result->bits[7] = scale_and_sign.bits[3];

  return error_mul;
}
