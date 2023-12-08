#include "../s21_string.h"

char *s21_strrchr(const char *str, int c) {
  const char *result = S21_NULL;
  if (c == 0) result = "";
  if (str != S21_NULL) {
    do {
      if (*str == (char)c) {
        result = str;
      }
    } while (*str++);
  }
  return (char *)result;
}
