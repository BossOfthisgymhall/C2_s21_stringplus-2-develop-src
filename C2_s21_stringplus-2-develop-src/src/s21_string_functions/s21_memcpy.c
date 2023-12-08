#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  s21_size_t index = 0;
  if (dest != S21_NULL && src != S21_NULL) {
    char *char_dest = (char *)dest;
    char *char_src = (char *)src;
    for (; index < n; index++) {
      char_dest[index] = char_src[index];
    }
  }
  return dest;
}
