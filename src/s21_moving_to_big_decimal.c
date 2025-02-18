#include "s21_decimal.h"

/////////////////////////////////////////////////////////////////////////////////////////////

void null_big_decimal(s21_big_decimal *num) {
  for (int i = 0; i < 8; i++) num->bits[i] = 0;
}

bool is_null_big_decimal(s21_big_decimal *num) {
  int flag = 1;
  for (int i = 0; i < 7; i++) {
    if (num->bits[i] != 0) {
      flag = 0;
      break;
    }
  }
  return flag;
}

/////////////////////////////////////////////////////////////////////////////////////////////

// void s21_big_decimal_bprint(s21_big_decimal *num) {
//   printf("\t\t[7]\n");
//   for (int i = BIG_BADGE; i >= 0; --i) {
//     putchar(s21_bit_get(num->bits, i) ? '1' : '0');
//     if (i != 0 && i % 4 == 0) {
//       printf(i % 32 == 0 ? " | " : " ");
//       if (i % 128 == 0) printf("\n");
//     }
//   }
//   printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[0]");
//   putchar('\n');
// }

/////////////////////////////////////////////////////////////////////////////////////////////

bool s21_bd_sign_get(s21_big_decimal *num) {
  return s21_bit_get(num->bits, BIG_BADGE);
}

void s21_bd_sign_set(s21_big_decimal *num, bool sign) {
  s21_set_bit(num->bits, BIG_BADGE, sign);
}

/////////////////////////////////////////////////////////////////////////////////////////////

unsigned int s21_bd_scale_get(s21_big_decimal *num) {
  bool sign = s21_bd_sign_get(num);
  s21_bd_sign_set(num, 0);
  unsigned int scale = num->bits[7] >> 16;
  s21_bd_sign_set(num, sign);
  return scale;
}

void s21_bd_scale_set(s21_big_decimal *num, unsigned int scale) {
  bool sign = s21_bd_sign_get(num);
  num->bits[7] = 0;
  num->bits[7] = scale << 16;
  s21_bd_sign_set(num, sign);
}

/////////////////////////////////////////////////////////////////////////////////////////////

int s21_big_is_equal(s21_big_decimal value_1, s21_big_decimal value_2) {
  int flag = 1;
  for (int i = 0; i < 8; i++) {
    if (!(value_1.bits[i] == value_2.bits[i])) {
      flag = 0;
      break;
    }
  }
  return flag;
}