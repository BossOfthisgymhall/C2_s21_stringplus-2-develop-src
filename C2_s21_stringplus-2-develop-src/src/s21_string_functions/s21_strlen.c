#include "../s21_string.h"

s21_size_t s21_strlen(const char *str) {
  int index = 0;
  for (; str[index] != '\0';) {
    index++;
  }
  return index;
}
