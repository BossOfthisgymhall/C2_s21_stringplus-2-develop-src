#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *result = S21_NULL;
  if (haystack != S21_NULL && needle != S21_NULL) {
    int length_haystack = s21_strlen(haystack),
        length_needle = s21_strlen(needle);
    int find = 0, index_haystack = 0, temp_id = -1;
    if (length_needle <= length_haystack) {
      for (int index_needle = 0;
           index_haystack < length_haystack && index_needle < length_needle;
           index_haystack++) {
        if (haystack[index_haystack] == needle[index_needle]) {
          find++;
          index_needle++;
          if (temp_id == -1) {
            temp_id = index_haystack + 1;
          }
        } else {
          find = 0;
          index_needle = 0;
          if (temp_id != -1) index_haystack = temp_id;
          temp_id = -1;
        }
      }
      if (find) {
        result = (char *)haystack + (index_haystack - find);
      }
    }
    if (needle[0] == '\0') result = (char *)haystack;
  }
  return result;
}
