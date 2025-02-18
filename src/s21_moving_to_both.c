#include "s21_decimal.h"

/////////////////////////////////////////////////////////////////////////////////////////////

u_int32_t s21_bit_get(u_int32_t *bits, size_t index) {
  u_int32_t bit_mask = (u_int32_t)1 << (index % 32);
  return bits[index / 32] & bit_mask ? 1 : 0;
}

void s21_set_bit(u_int32_t *bits, int digit, bool strive_for) {
  if (strive_for)
    s21_change_digit_value_to_1(bits, digit);
  else
    s21_change_digit_value_to_0(bits, digit);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void s21_change_digit_value_to_1(u_int32_t *bits, int digit) {
  unsigned int *bit = &bits[digit / 32];
  if (s21_bit_get(bits, digit) == 0) *bit = (*bit + (int)pow(2, digit % 32));
}

void s21_change_digit_value_to_0(u_int32_t *bits, int digit) {
  unsigned int *bit = &bits[digit / 32];
  if (s21_bit_get(bits, digit) == 1) *bit = (*bit - (int)pow(2, digit % 32));
}

/////////////////////////////////////////////////////////////////////////////////////////////

s21_decimal s21_convert_to_small(s21_big_decimal num) {  // good
  s21_decimal small = {{0, 0, 0, 0}};
  for (int i = 0; i < 3; i++) small.bits[i] = num.bits[i];
  small.bits[3] = num.bits[7];
  return small;
}

s21_big_decimal s21_convert_to_big(s21_decimal num) {
  s21_big_decimal big = {{0, 0, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < 3; i++) big.bits[i] = num.bits[i];
  big.bits[7] = num.bits[3];
  return big;
}
