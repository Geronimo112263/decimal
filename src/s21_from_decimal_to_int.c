#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  *dst = 0;
  s21_truncate(src, &src);
  if (src.bits[1] || src.bits[2]) {
    error = 1;
  } else
    convert_dec_to_int(src, dst, &error);
  return error;
}

void convert_dec_to_int(s21_decimal src, int *dst, int *err_flag) {
  int x = 0;
  for (int i = 0; i < 31; i++) {
    if (s21_bit_get(src.bits, i)) {
      x += pow(2, i);
    }
  }
  if (s21_sign_get(&src)) {
    if (s21_bit_get(src.bits, 31) && !x) {
      *dst = -2147483648;
    } else if (!s21_bit_get(src.bits, 31)) {
      *dst = x;
      *dst = -*dst;
    } else
      *err_flag = 1;
  } else if (!s21_sign_get(&src)) {
    if (s21_bit_get(src.bits, 31)) {
      *err_flag = 1;
    } else {
      *dst = x;
    }
  }
}
