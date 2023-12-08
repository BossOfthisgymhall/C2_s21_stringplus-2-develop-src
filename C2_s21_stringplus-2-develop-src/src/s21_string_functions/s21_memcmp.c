#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int flag = 1;
  int result = 0;
  if (str1 != S21_NULL && str2 != S21_NULL) {
    char *char_str1 = (char *)str1, *char_str2 = (char *)str2;
    for (s21_size_t index = 0; flag && index < n; index++) {
      if (char_str1[index] != char_str2[index]) {
        flag = 0;
        result = char_str1[index] - char_str2[index];
      }
    }
  }
  return result;
}
