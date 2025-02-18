#include "s21_decimal.h"

/////////////////////////////////////////////////////////////////////////////////////////////

// void s21_decimal_bprint(s21_decimal *num) {
//   for (int i = BADGE; i >= 0; --i) {
//     putchar(s21_bit_get(num->bits, i) ? '1' : '0');
//     if (i != 0 && i % 4 == 0) printf(i % 32 == 0 ? " | " : " ");
//   }
//   putchar('\n');
// }

/////////////////////////////////////////////////////////////////////////////////////////////

bool is_null(s21_decimal *num) {
  return num->bits[0] == 0 && num->bits[1] == 0 && num->bits[2] == 0 ? 1 : 0;
}

void null_decimal(s21_decimal *num) {
  for (int i = 0; i < 4; i++) num->bits[i] = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool s21_sign_get(s21_decimal *num) { return s21_bit_get(num->bits, BADGE); }

void s21_sign_set(s21_decimal *num, bool sign) {
  s21_set_bit(num->bits, BADGE, sign);
}

/////////////////////////////////////////////////////////////////////////////////////////////

unsigned int s21_scale_get(s21_decimal *num) {
  bool sign = s21_sign_get(num);
  s21_sign_set(num, 0);
  unsigned int scale = num->bits[3] >> 16;
  s21_sign_set(num, sign);
  return scale;
}

void s21_scale_set(s21_decimal *num, unsigned int scale) {
  bool sign = s21_sign_get(num);
  num->bits[3] = 0;
  num->bits[3] = scale << 16;
  s21_sign_set(num, sign);
}

/////////////////////////////////////////////////////////////////////////////////////////////