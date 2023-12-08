#include "../s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  char *result = (char *)str;
  for (s21_size_t index = 0; index < n; index++) {
    result[index] = c;
  }
  return (void *)result;
}
