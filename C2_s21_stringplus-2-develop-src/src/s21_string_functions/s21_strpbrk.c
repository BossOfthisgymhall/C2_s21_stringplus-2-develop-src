#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *result = S21_NULL;
  int length_str1 = s21_strlen(str1);
  int length_str2 = s21_strlen(str2);
  for (int index_str1 = 0, stop = 0; index_str1 < length_str1 && !stop;
       index_str1++) {
    for (int index_str2 = 0; index_str2 < length_str2 && !stop; index_str2++) {
      if (*str1 == *(str2 + index_str2)) {
        result = (char *)str1;
        stop = 1;
      }
    }
    if (!stop) str1++;
  }
  return result;
}