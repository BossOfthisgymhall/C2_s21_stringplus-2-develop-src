#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = S21_NULL;
  if (src != NULL && str != NULL) {
    size_t length_src = s21_strlen(src);
    size_t length_str = s21_strlen(str);
    size_t length_result = length_src + length_str;
    if (start_index <= length_src) {
      result = (char *)calloc(length_result + 1, sizeof(char));
      if (result != NULL) {
        for (size_t index_src = 0; index_src < start_index; index_src++) {
          result[index_src] = src[index_src];
        }
        for (size_t index_str = start_index, start_index_str = 0;
             start_index_str <= length_str; index_str++, start_index_str++) {
          result[index_str] = str[start_index_str];
        }
        for (size_t index_result = start_index + length_str,
                    index_src = start_index;
             index_result <= length_result; index_result++, index_src++) {
          result[index_result] = src[index_src];
        }
      }
    }
  }
  return result;
}
