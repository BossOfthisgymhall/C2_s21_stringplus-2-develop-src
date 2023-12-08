#include "../s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int flag = 1;
  int result = 0;
  if (str1 != S21_NULL && str2 != S21_NULL) {
    for (s21_size_t index = 0;
         (str1[index] != '\0' || str2[index] != '\0') && flag && index < n;) {
      if (str1[index] != str2[index]) {
        flag = 0;
        result = str1[index] - str2[index];
      } else
        index++;
    }
  }
  return result;
}
