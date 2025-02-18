#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  s21_decimal negative_one = {{1, 0, 0, 1 << 31}};
  int error = 0;
  if (is_null(&value) && s21_scale_get(&value) <= 28) {
    if (s21_sign_get(&value)) {
      *result = value;
      s21_sign_set(result, 0);
    } else {
      *result = value;
      s21_sign_set(result, 1);
    }
  } else {
    error = s21_mul(value, negative_one, result);
  }
  return error;
}