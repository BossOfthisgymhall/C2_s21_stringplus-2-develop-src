#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *result = S21_NULL;
  int flag = 0;
  if (str != S21_NULL) {
    for (s21_size_t index = 0; index < n && !flag; index++) {
      if (*((char *)str + index) == c) {
        result = ((char *)str + index);
        flag++;
      }
    }
  }
  return result;
}
