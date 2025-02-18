#include "s21_decimal.h"

int carina_derji(s21_decimal value) {
  int error = 0;
  check_work_scale_and_sign check;
  check.zero = value.bits[3];
  check.zero_two = value.bits[3] >> 24;
  check.scale = value.bits[3] >> 16;
  int flag_scale = check.scale > 28 ? 1 : 0;
  if (check.zero || check.zero_two || flag_scale) {
    error = 5;
  }
  return error;
}