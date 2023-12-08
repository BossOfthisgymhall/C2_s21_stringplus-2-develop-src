#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  int index = 0;
  char *result = S21_NULL;
  if (src != S21_NULL && dest != S21_NULL) {
    for (; (s21_size_t)index < n && (*src != '\0' || *dest != '\0');
         src++, dest++) {
      *dest = *src;
      index++;
    }
    result = (char *)dest - index;
  }
  return result;
}
