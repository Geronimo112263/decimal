#include "s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = s21_comparison(value_1, value_2);
  if (res == 1) {
    res = 1;
  } else
    res = 0;
  return res;
}