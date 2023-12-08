#include "../s21_string.h"

int find_coincidence(const char src, const char *trim_chars, int length_trim) {
  int result = 0;
  for (int index_trim = 0; index_trim < length_trim && !result; index_trim++) {
    if (src == trim_chars[index_trim]) result = 1;
  }
  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result_trim = NULL;
  if (src) {
    if (trim_chars && trim_chars[0]) {
      int left = 0, length_src = s21_strlen(src), right = length_src - 1,
          length_trim = s21_strlen(trim_chars);
      while ((find_coincidence(src[left], trim_chars, length_trim))) left++;
      if (left < right) {
        while (find_coincidence(src[right], trim_chars, length_trim)) right--;
      }
      int new_length_trim = (right - left) + 1;
      result_trim = calloc(new_length_trim + 1, sizeof(char));
      if (result_trim) {
        for (int index_src = left, index_result_trim = 0; index_src <= right;
             index_src++, index_result_trim++) {
          result_trim[index_result_trim] = src[index_src];
        }
        result_trim[new_length_trim] = '\0';
      } else {
        result_trim = "";
      }
    } else {
      result_trim = s21_trim(src, "\t\n");
    }
  }
  return result_trim;
}
