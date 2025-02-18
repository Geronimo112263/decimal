#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  s21_err_codes err_flag = OK;
  if (!dst || isnan(src) || isinf(src)) return CONVERSATION_ERR;
  if (fabs(src) < MIN_FLOAT && src != 0.0) {
    err_flag = CONVERSATION_ERR;
    null_decimal(dst);
  } else if (fabs(src) > MAX_FLOAT) {
    err_flag = CONVERSATION_ERR;
  } else {
    convert_flt_to_dec(src, dst);
  }
  return err_flag;
}

void convert_flt_to_dec(float src, s21_decimal *dst) {
  null_decimal(dst);
  if (!(src == 0)) {
    int sign = src < 0 ? 1 : 0;
    s21_sign_set(dst, sign);
    char temp[32] = {0};
    sprintf(temp, "%.6e", fabs(src));
    for (int i = 1; i < 32; i++) {  // Записали два инта
      temp[i] = temp[i + 1];
    }
    int mantisse = 0, scale = 0;
    sscanf(temp, "%de%d", &mantisse, &scale);
    scale += -6;  // Записали мантиссу * 10^6 и скейл
    int reminder = 0;
    while (mantisse > 0) {
      reminder = mantisse % 10;
      if (reminder == 0 && scale < 0)  // Убираем незначащие нули
        scale++;
      else
        break;
      mantisse /= 10;
    }
    if (scale > 0) {  // Переводим целую часть в дец и битово умножаем на 10
                      // (переполнение мантисы)
      dst->bits[0] = mantisse;
      while (scale > 0) {
        *dst = s21_mul_by_ten(*dst);
        scale--;
      }
      s21_scale_set(dst, scale);
    } else if (scale <
               -28) {  // Делим на 10 и округляем результат, если скейл >28
      double mantisse_f = (double)mantisse;
      while (scale < -28) {
        mantisse_f = round(mantisse_f / 10);
        scale++;
      }
      dst->bits[0] = (int)mantisse_f;
      scale = abs(scale);
      s21_scale_set(dst, scale);
    } else {
      scale = abs(scale);
      dst->bits[0] = mantisse;
      s21_scale_set(dst, scale);
    }
  }
}

s21_decimal s21_mul_by_ten(s21_decimal case_value) {
  s21_decimal result = {{0}};
  s21_decimal ref = {{0b1010, 0, 0, 0}};
  s21_mul(case_value, ref, &result);
  return result;
}
