#include "../s21_string.h"

void *s21_to_upper(const char *string) {
  char *string_copy_upper = S21_NULL;
  if (string != NULL) {
    s21_size_t length = s21_strlen(string);
    string_copy_upper = (char *)calloc(length + 1, sizeof(char));
    if (string_copy_upper != NULL) {
      for (s21_size_t index = 0; index < length; index++) {
        if (string[index] >= 'a' && string[index] <= 'z') {
          string_copy_upper[index] = string[index] - 32;
        } else {
          string_copy_upper[index] = string[index];
        }
      }
    }
  }
  return string_copy_upper;
}
