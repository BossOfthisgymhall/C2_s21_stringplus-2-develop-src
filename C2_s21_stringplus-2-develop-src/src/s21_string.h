#ifndef _S21_STRING_H_
#define _S21_STRING_H_

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define s21_size_t unsigned long long
#define S21_NULL ((void *)0)

#define BUF_SIZE 1024

typedef struct flags {
  int error;
} flag_error;

typedef struct {
  int minus;
  int plus;
  int space;
  int accuracy;
  int width;
  char lenght;
  int check_point;
  int check_zero;
} description;

typedef struct {
  int count;
  int combined_width;
  unsigned long long int integer;
  int len;
  int negative_num;
  int check_unsigned;
  int checkpoint_count;
  long double checkpoint_f;
} options;

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strcpy(char *dest, const char *src);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

int s21_sprintf(char *str, const char *format, ...);

/**************************************************
 *           FUNCTIONS FOR SPRINTF                *
 **************************************************/

void zero_options(options *parametrs);
char *check_flags(char *str, description *spec);
char *check_width(char *str, description *spec, flag_error *end_programm);
char *check_accuracy(char *str, description *spec, flag_error *end_programm);
char *check_lenght(char *str, description *spec, flag_error *end_programm);
int search_for_a_specifier(char *tmp_format, char *str, va_list factor,
                           description *spec, options *parametrs,
                           flag_error *end_programm);
int processing_for_int(char *str, va_list factor, description *spec,
                       options *parametrs);
int processing_for_float(char *str, va_list factor, description *spec,
                         options *parametrs, flag_error *end_programm);
int processing_for_unsigned(char *str, va_list factor, description *spec,
                            options *parametrs);
int processing_for_string(char *str, va_list factor, description *spec,
                          int count);
int insert_a_line(description *spec, int count, int len, char *str,
                  char *string_arg);
int processing_for_char(char *str, va_list factor, description *spec,
                        int count);
int processing_for_percentage(char *str, description *spec, int count);
int insert_spaces(char *str, int width, int count, int c);
char *get_a_number(char *str, int *number, flag_error *end_programm,
                   int width_with_zero, int *flag_zero);
int get_int_from_char(int size_buffer, char *buffer, int number);
char transfer_int(int a);
int transfer_char(char b);
int d_function(char *str, unsigned long long int d, int count);
int conditions_for_int(description *spec, char *str, options *parametrs);
void deep_conditions_for_int(description *spec, options *parametrs, char *str);
int insert_spaces_with_minus_or_plus(char *str, options *parametrs, char c,
                                     char sign);
int negative_num_function(description *spec, char *str, options *parametrs);
int accuracy_bigger_len_function(description *spec, char *str,
                                 options *parametrs);
int space_function(description *spec, char *str, options *parametrs);
int f_function(char *str, long double f, int count, description *spec,
               flag_error *end_programm);
void conditions_for_float(description *spec, char *str, options *parametrs);
void function_for_long_f(long double f, flag_error *end_programm);
int fractional_part_of_a_number(char *str, long double f, options *parametrs,
                                description *spec);
int rounding_check(char *str, long double f, options *parametrs,
                   description *spec);
unsigned long long int s21_roundl(long double f);

/**************************************************
 *               FUNCTIONS FOR C#                 *
 **************************************************/

void *s21_to_upper(const char *string);
void *s21_to_lower(const char *string);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
int find_coincidence(const char src, const char *trim_chars, int lenght_trim);
void *s21_trim(const char *src, const char *trim_chars);
#endif  // _S21_STRING_H_
