#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_err_codes err_flag = OK;
  null_decimal(dst);
  if (!dst || isnan((double)src) || isinf((double)src))
    err_flag = CONVERSATION_ERR;
  if (!err_flag) {
    if (src >= 0)
      dst->bits[0] = src;
    else {
      dst->bits[0] = ~src + 1;
      s21_sign_set(dst, 1);
    }
  }
  return err_flag;
}
