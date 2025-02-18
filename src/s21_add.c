#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  null_decimal(result);
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  s21_big_decimal big_result;
  big_add(big_value_1, big_value_2, &big_result);
  int error = overflow(&big_result);
  if (!error) {
    *result = s21_convert_to_small(big_result);
  }
  return error;
}

int big_add(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *result) {
  null_big_decimal(result);
  big_normalization(&value_1, &value_2);
  int bit_value_1 = 0, bit_value_2 = 0, cont = 0, res = 0, error_add = 0;
  int sign_val_1 = s21_bit_get(value_1.bits, BIG_BADGE);
  int sign_val_2 = s21_bit_get(value_2.bits, BIG_BADGE);
  s21_decimal scale_and_sign;
  scale_and_sign_for_arithmetic(s21_convert_to_small(value_1),
                                s21_convert_to_small(value_2), &scale_and_sign);
  if ((sign_val_1 && !sign_val_2) || (!sign_val_1 && sign_val_2)) {
    value_1.bits[7] = 0;
    value_2.bits[7] = 0;
    big_sub(value_1, value_2, result);
  } else {
    for (int i = 0; i <= 223; i++) {
      bit_value_1 = s21_bit_get(value_1.bits, i);
      bit_value_2 = s21_bit_get(value_2.bits, i);
      res = (bit_value_1 + bit_value_2 + cont) % 2;
      cont = (bit_value_1 + bit_value_2 + cont) / 2;
      s21_set_bit(result->bits, i, res);
    }
  }
  result->bits[7] = scale_and_sign.bits[3];
  return error_add;
}
