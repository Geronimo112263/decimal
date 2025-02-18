#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  null_decimal(result);
  if (carina_derji(value_1) || carina_derji(value_2)) {
    return 4;
  }
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  s21_big_decimal big_result;
  int error;
  if (big_div(big_value_1, big_value_2, &big_result) == 3)
    error = 3;
  else
    error = overflow(&big_result);
  if (!error) {
    *result = s21_convert_to_small(big_result);
  }
  return error;
}

int big_div(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal* result) {
  null_big_decimal(result);
  int scale_a = s21_bd_scale_get(&value_1);
  int scale_b = s21_bd_scale_get(&value_2);
  bool sign_a = s21_bd_sign_get(&value_1);
  bool sign_b = s21_bd_sign_get(&value_2);
  value_1.bits[7] = 0, value_2.bits[7] = 0;
  int label, diff, err = 0, scale = 0, flag_complete_div = 0;
  null_big_decimal(result);
  unsigned int digit_b = big_first_digit(value_2);
  s21_big_decimal temp, res = {0};
  s21_big_decimal a[300] = {{{0, 0, 0, 0, 0, 0, 0, 0}}};
  unsigned int first_num_bfd = 0, num_size = 0;
  bool break_flag = 0;
  if (!is_null_big_decimal(&value_2) && !is_null_big_decimal(&value_1)) {
    while (REPID) {
      flag_complete_div++;
      if (big_first_digit(value_1) >= digit_b) {
        temp_calc(&diff, &value_1, &value_2, &temp, &digit_b, &label, &res,
                  &scale, &a[scale], &break_flag);
      } else {
        value_1 = s21_big_mul_by_ten(value_1);
        null_big_decimal(&(a[scale]));
        null_big_decimal(&res);
        scale++;
      }
      if (flag_complete_div == 1)
        first_num_bfd = big_first_digit(a[0]);
      else {
        num_size = first_num_bfd + scale * 4;
        if (num_size > 210 || scale + scale_a - scale_b > 28) {
          scale--;
          break_flag = 1;
        }
      }
      if (break_flag) break;
    }
    mantissa_big_calc(scale, a, result);
    scale_big_calc(scale, scale_a, scale_b, result);
    if (sign_a + sign_b == 1) s21_bd_sign_set(result, 1);
  } else if (is_null_big_decimal(&value_1))
    err = 0;
  else
    err = 3;
  return err;
}

void temp_calc(int* diff, s21_big_decimal* value_1, s21_big_decimal* value_2,
               s21_big_decimal* temp, unsigned int* digit_b, int* label,
               s21_big_decimal* res, int* scale, s21_big_decimal* array_element,
               bool* break_flag) {
  *diff = big_first_digit(*value_1) - *digit_b;
  *temp = big_shift_backward(*value_1, *diff);
  for (*label = *diff - 1; *label > -1; (*label)--) {
    if (s21_big_comparison(*temp, *value_2) == -1 ||
        is_null_big_decimal(temp)) {
      s21_big_append_to_temp(temp, *value_1, *label);
      s21_big_append_to_res(res, 0);
    } else {
      big_sub(*temp, *value_2, temp);
      s21_big_append_to_temp(temp, *value_1, *label);
      s21_big_append_to_res(res, 1);
    }
  }
  if (big_checking_remainder(scale, array_element, *temp, *value_2, res,
                             value_1))
    *break_flag = 1;
}

void scale_big_calc(int scale_from_while, int scale_a, int scale_b,
                    s21_big_decimal* result) {
  int new_scale = scale_from_while + scale_a - scale_b;
  s21_bd_scale_set(result, new_scale);
}

bool big_checking_remainder(int* scale, s21_big_decimal* array_element,
                            s21_big_decimal temp, s21_big_decimal value_2,
                            s21_big_decimal* res, s21_big_decimal* value_1) {
  bool flag_break = 0;
  if (s21_big_is_equal(temp, value_2)) {
    s21_big_append_to_res(res, 1);
    flag_break = 1;
  } else if (!is_null_big_decimal(&temp)) {
    if (s21_big_comparison(temp, value_2) == 1) {
      big_sub(temp, value_2, &temp);
      s21_big_append_to_res(res, 1);
    } else
      s21_big_append_to_res(res, 0);
    *value_1 = s21_big_mul_by_ten(temp);
    (*scale)++;
  } else {
    s21_big_append_to_res(res, 0);
    flag_break = 1;
  }
  *array_element = *res;
  null_big_decimal(res);
  return flag_break;
}

void mantissa_big_calc(int scale, s21_big_decimal a[],
                       s21_big_decimal* result) {
  int res_counter = 0;
  int j, k;
  s21_big_decimal val;
  null_big_decimal(&val);
  for (j = scale; j > 0; j--) {
    if (!is_null_big_decimal(&(a[res_counter]))) {
      val = s21_big_mul_by_ten(a[res_counter]);
      for (k = 0; k < j - 1; k++) {
        val = s21_big_mul_by_ten(val);
      }
      big_add(*result, val, result);
      null_big_decimal(&val);
    }
    res_counter++;
  }
  big_add(*result, a[res_counter], result);
}

unsigned int big_first_digit(s21_big_decimal num) {
  unsigned int digit = -1;
  for (int i = 223; i >= 0; --i) {
    if (s21_bit_get(num.bits, i) == 1) {
      digit = i;
      break;
    }
  }
  return digit;
}

s21_big_decimal big_shift_backward(s21_big_decimal num, int moving) {
  s21_big_decimal result = {{0, 0, 0, 0}};
  int scale = s21_bd_scale_get(&num);
  int sign = s21_bd_sign_get(&num);
  for (int i = moving; i <= 223; i++) {
    s21_set_bit(result.bits, i - moving, s21_bit_get(num.bits, i));
  }
  s21_bd_scale_set(&result, scale);
  s21_bd_sign_set(&result, sign);
  return result;
}

void s21_big_append_to_temp(s21_big_decimal* temp, s21_big_decimal value_1,
                            int label) {
  big_shift(*temp, 1, temp);
  s21_set_bit(temp->bits, 0, s21_bit_get(value_1.bits, label));
}

void s21_big_append_to_res(s21_big_decimal* res, bool bool_res) {
  big_shift(*res, 1, res);
  s21_set_bit(res->bits, 0, bool_res);
}

s21_big_decimal s21_big_mul_by_ten(
    s21_big_decimal case_value) {  //без учета overflow, работает
  s21_big_decimal step_one;
  s21_big_decimal step_three;
  big_shift(case_value, 1, &step_one);
  big_shift(case_value, 3, &step_three);
  big_add(step_one, step_three, &case_value);
  return case_value;
}

s21_big_decimal s21_div_by_ten_without_rem(s21_big_decimal* result) {
  unsigned int bit_7 = result->bits[7];
  result->bits[7] = 0;
  s21_big_decimal big_ten = {0};
  big_ten.bits[0] = 10;
  s21_big_decimal value_1 = *result;
  s21_big_decimal value_2 = big_ten;
  null_big_decimal(result);
  unsigned int digit_b = big_first_digit(value_2);
  s21_big_decimal temp;
  int label, diff, scale = 0;
  s21_big_decimal res = {0};
  s21_big_decimal a[300] = {{{0, 0, 0, 0, 0, 0, 0, 0}}};
  s21_big_decimal rem = {0};
  int flag_complete_div = 0;
  bool break_flag = 0;
  if (!is_null_big_decimal(&value_2) && !is_null_big_decimal(&value_1)) {
    while (REPID) {
      flag_complete_div++;
      if (big_first_digit(value_1) >= digit_b) {
        temp_calc(&diff, &value_1, &value_2, &temp, &digit_b, &label, &res,
                  &scale, &a[scale], &break_flag);
        if (break_flag) break;
      } else {
        value_1 = s21_big_mul_by_ten(value_1);
        null_big_decimal(&(a[scale]));
        null_big_decimal(&res);
        scale++;
      }
    }
    mantissa_big_calc(scale, a, result);
    if (flag_complete_div != 1) {
      rem = a[scale];
      big_sub(*result, rem, result);
      big_div(*result, big_ten, result);
    } else {
      *result = a[scale];
    }
    result->bits[7] = bit_7;
  } else
    null_big_decimal(result);
  return rem;
}
