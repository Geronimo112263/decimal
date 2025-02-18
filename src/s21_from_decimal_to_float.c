#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  s21_err_codes err_flag = OK;
  if (!dst) err_flag = CONVERSATION_ERR;
  if (!err_flag) convert_dec_to_float(src, dst);
  return err_flag;
}

void convert_dec_to_float(s21_decimal src, float *dst) {
  double tmp = 0.0;
  int scale = s21_scale_get(&src);
  for (int i = 0; i < 96; i++) {
    if (s21_bit_get(src.bits, i) != 0) tmp += pow(2.0, i);
  }
  if (s21_sign_get(&src)) tmp *= -1.0;
  tmp /= pow(10, scale);
  *dst = (float)tmp;
}
