#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  if (src != S21_NULL && n > 0) {
    s21_size_t lenght_dest = s21_strlen(dest);
    for (s21_size_t index_src = 0, index_dest = lenght_dest;
         src && index_src < n; index_src++, index_dest++) {
      dest[index_dest] = src[index_src];
    }
  }
  return dest;
}
