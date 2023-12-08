#include "../s21_string.h"

void *s21_to_lower(const char *string) {
  char *string_copy_lower = S21_NULL;
  if (string != NULL) {
    s21_size_t length = s21_strlen(string);
    string_copy_lower = (char *)calloc(length + 1, sizeof(char));
    if (string_copy_lower != NULL) {
      for (s21_size_t index = 0; index < length; index++) {
        if (string[index] >= 'A' && string[index] <= 'Z') {
          string_copy_lower[index] = string[index] + 32;
        } else {
          string_copy_lower[index] = string[index];
        }
      }
    }
  }
  return string_copy_lower;
}
