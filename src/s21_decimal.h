#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BADGE 127
#define BIG_BADGE 255
#define REPID 1

#define MIN_FLOAT 1e-28
#define MAX_FLOAT 7.9228163e28f

#define u_int32_t unsigned

typedef struct check_work_scale_and_sign {
  u_int32_t zero : 16;
  u_int32_t zero_two : 7;
  u_int32_t scale : 8;
} check_work_scale_and_sign;

typedef struct decimal {
  unsigned bits[4];
} s21_decimal;

typedef struct big_decimal {
  u_int32_t bits[8];
} s21_big_decimal;

typedef enum Error_codes { OK, CONVERSATION_ERR } s21_err_codes;

//Операции сравнения
int s21_is_less(s21_decimal val_1, s21_decimal val_2);
int s21_is_greater(s21_decimal val_1, s21_decimal val_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);

int s21_comparison(s21_decimal value_1, s21_decimal value_2);
int s21_big_comparison(s21_big_decimal big_value_1,
                       s21_big_decimal big_value_2);
int s21_big_is_equal(s21_big_decimal value_1, s21_big_decimal value_2);

//Арифметические операции
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int big_add(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *result);
int big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *result);
int big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *result);
int big_div(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *result);
void scale_big_calc(int scale_from_while, int scale_a, int scale_b,
                    s21_big_decimal *result);
bool big_checking_remainder(int *scale, s21_big_decimal *array_element,
                            s21_big_decimal temp, s21_big_decimal value_2,
                            s21_big_decimal *res, s21_big_decimal *value_1);
void mantissa_big_calc(int scale, s21_big_decimal a[], s21_big_decimal *result);
unsigned int big_first_digit(s21_big_decimal num);
s21_big_decimal big_shift_backward(s21_big_decimal num, int moving);
void s21_big_append_to_temp(s21_big_decimal *temp, s21_big_decimal value_1,
                            int label);
void s21_big_append_to_res(s21_big_decimal *res, bool bool_res);
s21_big_decimal s21_big_mul_by_ten(s21_big_decimal case_value);
s21_big_decimal s21_div_by_ten_without_rem(s21_big_decimal *case_value);

// 0 - OK
// 1 - число слишком велико или равно бесконечности
// 2 - число слишком мало или равно отрицательной бесконечности
// 3 - деление на 0

//Округление и тп
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// 0 - OK
// 1 - ошибка вычисления

//Преобразователи
int s21_from_int_to_decimal(int src,
                            s21_decimal *dst);  //++++++++++проходит все тесты
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// 0 - OK
// 1 - ошибка конвертации

//проверка scale , нормализация
int checking_scale(
    s21_decimal *value_1,
    s21_decimal *value_2);  //++    в дальнейшем возможно потребуется доработка
void normalization(s21_decimal *value_1, s21_decimal *value_2);  //++
void s21_scale_add(s21_decimal *value);                          //++
void shift(s21_decimal num, int moving, s21_decimal *result);    //++

void scale_and_sign_for_arithmetic(s21_decimal value_1, s21_decimal value_2,
                                   s21_decimal *scale_and_sign);

void scale_and_sign_for_mul(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *scale_and_sign);

//нужные маленькие функции по передвижению по децимал
// void s21_decimal_bprint(s21_decimal *num);
bool is_null(s21_decimal *num);
void null_decimal(s21_decimal *num);

bool s21_sign_get(s21_decimal *num);
void s21_sign_set(s21_decimal *num, bool sign);

unsigned int s21_scale_get(s21_decimal *num);
void s21_scale_set(s21_decimal *num, unsigned int scale);

//босс переходов
u_int32_t s21_bit_get(u_int32_t *bits, size_t index);
void s21_set_bit(u_int32_t *bits, int digit, bool strive_for);
void s21_change_digit_value_to_1(u_int32_t *bits, int digit);
void s21_change_digit_value_to_0(u_int32_t *bits, int digit);
s21_decimal s21_convert_to_small(s21_big_decimal num);
s21_big_decimal s21_convert_to_big(s21_decimal num);

//передвижение по большому децималу
void null_big_decimal(s21_big_decimal *num);
// void s21_big_decimal_bprint(s21_big_decimal *num);
bool s21_bd_sign_get(s21_big_decimal *num);
void s21_bd_sign_set(s21_big_decimal *num, bool sign);
unsigned int s21_bd_scale_get(s21_big_decimal *num);
void s21_bd_scale_set(s21_big_decimal *num, unsigned int scale);
bool is_null_big_decimal(s21_big_decimal *num);

//нормализация для большого децимала
void big_normalization(s21_big_decimal *value_1, s21_big_decimal *value_2);
int big_checking_scale(s21_big_decimal *value_1, s21_big_decimal *value_2);
void big_scale_add(s21_big_decimal *value);
void big_shift(s21_big_decimal num, int moving, s21_big_decimal *result);

void temp_calc(int *diff, s21_big_decimal *value_1, s21_big_decimal *value_2,
               s21_big_decimal *temp, unsigned int *digit_b, int *label,
               s21_big_decimal *res, int *scale, s21_big_decimal *array_element,
               bool *break_flag);
unsigned int first_digit(s21_decimal num);
s21_decimal shift_backward(s21_decimal num, int moving);
void s21_append_to_temp(s21_decimal *temp, s21_decimal value_1, int label);
void s21_append_to_res(s21_decimal *res, bool bool_res);
s21_decimal s21_mul_by_ten(s21_decimal case_value);

void convert_dec_to_float(s21_decimal src, float *dst);
void convert_dec_to_int(s21_decimal src, int *dst, int *err_flag);
void convert_flt_to_dec(float src, s21_decimal *dst);
s21_decimal s21_mul_by_ten(s21_decimal case_value);
void normalize_scale(int *base, int *scale);

int overflow(s21_big_decimal *value);
int checking_mantise(s21_big_decimal value);
void ochko_banka(s21_big_decimal *result);

int carina_derji(s21_decimal value);

#endif