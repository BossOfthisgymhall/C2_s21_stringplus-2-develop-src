#include "../s21_string.h"

char *s21_strchr(const char *str, int c) {
  const char *result = S21_NULL;
  int stop = 0;
  if (c == 0) result = "";
  if (str != S21_NULL) {
    do {
      if (*str == (char)c) {
        result = str;
        stop++;
      }
    } while (*str++ && !stop);
  }
  return (char *)result;
}