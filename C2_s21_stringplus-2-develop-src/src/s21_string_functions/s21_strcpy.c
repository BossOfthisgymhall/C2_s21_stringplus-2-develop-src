#include "../s21_string.h"

char *s21_strcpy(char *dest, const char *src) {
  int index = 0;
  char *result = S21_NULL;
  if (dest != S21_NULL && src != S21_NULL) {
    for (; *src != '\0' || *dest != '\0'; src++, dest++) {
      *dest = *src;
      index++;
    }
    result = (char *)dest - index;
  }
  return result;
}
