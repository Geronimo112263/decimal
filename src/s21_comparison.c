#include "s21_decimal.h"

int s21_comparison(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  big_normalization(&big_value_1, &big_value_2);
  int res = 0;
  int flag_stop = 0;
  unsigned int sign_value_1 = s21_bit_get(value_1.bits, BADGE);
  unsigned int sign_value_2 = s21_bit_get(value_2.bits, BADGE);
  if (is_null(&value_1) && is_null(&value_2)) {
    res = 0;
  } else if (sign_value_1 && !sign_value_2) {
    res = -1;
  } else if (!sign_value_1 && sign_value_2) {
    res = 1;
  } else {
    int bit_value_1 = 0, bit_value_2 = 0;
    for (int i = 223; i >= 0 && !flag_stop; i--) {
      bit_value_1 = s21_bit_get(big_value_1.bits, i);
      bit_value_2 = s21_bit_get(big_value_2.bits, i);
      if (bit_value_1 != bit_value_2) {
        flag_stop = 1;
        if (bit_value_1 > bit_value_2) {
          res = 1;
        } else
          res = -1;
      }
    }
  }
  if (sign_value_1 && sign_value_2 && flag_stop) {
    if (res == 1) {
      res = -1;
    } else
      res = 1;
  }
  return res;
}

int s21_big_comparison(s21_big_decimal big_value_1,
                       s21_big_decimal big_value_2) {
  big_normalization(&big_value_1, &big_value_2);
  int res = 0;
  int flag_stop = 0;
  unsigned int sign_value_1 = s21_bd_sign_get(&big_value_1);
  unsigned int sign_value_2 = s21_bd_sign_get(&big_value_2);
  if (is_null_big_decimal(&big_value_1) && is_null_big_decimal(&big_value_2)) {
    res = 0;
  } else if (sign_value_1 && !sign_value_2) {
    res = -1;
  } else if (!sign_value_1 && sign_value_2) {
    res = 1;
  } else {
    int bit_value_1 = 0, bit_value_2 = 0;
    for (int i = 223; i >= 0 && !flag_stop; i--) {
      bit_value_1 = s21_bit_get(big_value_1.bits, i);
      bit_value_2 = s21_bit_get(big_value_2.bits, i);
      if (bit_value_1 != bit_value_2) {
        flag_stop = 1;
        if (bit_value_1 > bit_value_2) {
          res = 1;
        } else
          res = -1;
      }
    }
  }
  if (sign_value_1 && sign_value_2 && flag_stop) {
    if (res == 1) {
      res = -1;
    } else
      res = 1;
  }
  return res;
}
