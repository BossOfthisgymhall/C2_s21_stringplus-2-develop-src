#include "../s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *point_str = S21_NULL;
  char *result_str = S21_NULL;
  if (*delim != '\0') {
    if (str != S21_NULL) {
      point_str = str;
    }
    if (point_str == S21_NULL || *point_str == '\0') {
      result_str = S21_NULL;
    }
    while (*point_str && s21_strchr(delim, *point_str)) {
      point_str++;
    }
    if (*point_str) {
      char *delim_buf = point_str;
      while (point_str && !s21_strchr(delim, *point_str)) {
        point_str++;
      }
      if (*point_str) {
        *point_str = '\0';
        point_str++;
      }
      result_str = delim_buf;
    }
  } else if (str != S21_NULL && *str != '\0') {
    result_str = str;
  }
  return result_str;
}
