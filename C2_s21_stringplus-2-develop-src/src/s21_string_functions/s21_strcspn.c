#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  int flag = 1;
  int index_str1 = 0;
  for (; str1[index_str1] != '\0' && flag;) {
    for (int index_str2 = 0; str2[index_str2] != '\0'; index_str2++) {
      if (str1[index_str1] == str2[index_str2]) flag = 0;
    }
    if (flag) index_str1++;
  }
  return index_str1;
}
