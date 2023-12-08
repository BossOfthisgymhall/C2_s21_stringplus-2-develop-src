#include "../s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  // ввод структуры ошибок при неправильном вводе спецификаторов
  flag_error end_programm = {0};
  va_list factor;
  va_start(factor, format);
  options parametrs = {0};
  for (char *tmp_format = (char *)format; *tmp_format && !end_programm.error;
       tmp_format++, parametrs.count++) {
    zero_options(&parametrs);
    // Структура со всеми флагами и спецификаторами
    description spec = {0};
    // точность = -2 (в случае, если точность не указана и нет '.')
    spec.accuracy = -2;
    if (*tmp_format != '%') {
      *(str + parametrs.count) = *tmp_format;
      continue;
    }

    tmp_format = check_flags(tmp_format, &spec);
    tmp_format = check_width(tmp_format, &spec, &end_programm);
    if (*tmp_format == '.') {
      spec.check_point = 1;
      tmp_format = check_accuracy(tmp_format, &spec, &end_programm);
    }
    if (!end_programm.error) {
      tmp_format = check_lenght(tmp_format, &spec, &end_programm);
      parametrs.count = search_for_a_specifier(tmp_format, str, factor, &spec,
                                               &parametrs, &end_programm);
    }
  }
  *(str + parametrs.count) = '\0';
  va_end(factor);
  return (end_programm.error ? -1 : parametrs.count);
}

void zero_options(options *parametrs) {
  parametrs->combined_width = 0;
  parametrs->integer = 0;
  parametrs->len = 0;
  parametrs->negative_num = 0;
  parametrs->check_unsigned = 0;
  parametrs->checkpoint_count = 0;
}

// Проверка флагов +, -, (пробел)
char *check_flags(char *str, description *spec) {
  str++;
  while (*str == ' ' || *str == '+' || *str == '-') {
    if (*str == ' ') {
      spec->space = 1;
      str++;
    }
    if (*str == '+') {
      spec->plus = 1;
      str++;
    }
    if (*str == '-') {
      spec->minus = 1;
      str++;
    }
  }
  return str;
}

// Проверка ширины
char *check_width(char *str, description *spec, flag_error *end_programm) {
  int width = 0;
  int flag_zero = 0;
  str = get_a_number(str, &width, end_programm, 1, &flag_zero);
  spec->width = width;
  spec->check_zero = flag_zero;
  return str;
}

// Проверка точности
char *check_accuracy(char *str, description *spec, flag_error *end_programm) {
  // точность = -1 (в случае, если есть '.', но точность не указана)
  int accuracy = -1;
  int flag_zero = 0;
  str++;
  str = get_a_number(str, &accuracy, end_programm, 0, &flag_zero);
  spec->accuracy = accuracy;
  return str;
}

// Проверка флагов длины
char *check_lenght(char *str, description *spec, flag_error *end_programm) {
  if (*str == 'h' || *str == 'l' || *str == 'L') {
    spec->lenght = *str;
    str++;
  }
  if (*str == 'l') {
    str++;
    if (*str == 'u' || *str == 'd')
      spec->lenght = 'U';
    else
      end_programm->error = 1;  // не тот спецификатор
  }
  return str;
}

int search_for_a_specifier(char *tmp_format, char *str, va_list factor,
                           description *spec, options *parametrs,
                           flag_error *end_programm) {
  switch (*tmp_format) {
    case 'd':
      parametrs->count = processing_for_int(str, factor, spec, parametrs);
      break;
    case 'f':
      parametrs->count =
          processing_for_float(str, factor, spec, parametrs, end_programm);
      break;
    case 'u':
      parametrs->count = processing_for_unsigned(str, factor, spec, parametrs);
      break;
    case 's':
      parametrs->count =
          processing_for_string(str, factor, spec, parametrs->count);
      break;
    case 'c':
      parametrs->count =
          processing_for_char(str, factor, spec, parametrs->count);
      break;
    case '%':
      parametrs->count = processing_for_percentage(str, spec, parametrs->count);
      break;
    default:
      fprintf(stderr, "Incorrect specifier\n");
      end_programm->error = 1;
      str[0] = '\0';
      break;
  }
  return parametrs->count;
}

int processing_for_int(char *str, va_list factor, description *spec,
                       options *parametrs) {
  long long int n = 0;
  if ((spec->accuracy == -1 || spec->accuracy == -2)) {
    spec->accuracy = 0;
  }
  long long int d_num = 0;
  d_num = va_arg(factor, long long int);
  if (spec->lenght == 'h') {
    d_num = (short int)d_num;
  } else if (spec->lenght == 0) {
    d_num = (int)d_num;
  }
  // условие для отрицательного инта - проверка на знаки

  if (d_num < 0) {
    parametrs->negative_num = 1;
    d_num = -d_num;
  }
  parametrs->integer = d_num;
  n = parametrs->integer;
  for (; n != 0; n = n / 10, parametrs->len++)
    ;

  if (d_num == 0) parametrs->checkpoint_count = 1;
  parametrs->count = conditions_for_int(spec, str, parametrs);
  if (!(parametrs->integer == 0 && spec->check_point == 1)) {
    parametrs->count = d_function(str, parametrs->integer, parametrs->count);
  }  // главная функция для инта
  if (spec->minus && spec->width) {
    if (spec->accuracy > parametrs->len)
      parametrs->len += (spec->accuracy - parametrs->len);
    if (spec->space || (!parametrs->negative_num && !spec->accuracy &&
                        !spec->plus && parametrs->checkpoint_count))
      parametrs->len += 1;
    if (parametrs->negative_num) parametrs->len += 1;
    if (parametrs->negative_num && spec->space && !spec->accuracy) {
      parametrs->len -= 1;
    }
    parametrs->count = insert_spaces(str, (spec->width - parametrs->len),
                                     parametrs->count, ' ');
  }
  parametrs->count--;
  return parametrs->count;
}

int processing_for_float(char *str, va_list factor, description *spec,
                         options *parametrs, flag_error *end_programm) {
  if (spec->accuracy == -1) {
    spec->accuracy = 0;
  } else if (spec->accuracy == -2) {
    spec->accuracy = 6;
  }
  long double f = 0;
  parametrs->len = 0;
  if (spec->lenght == 'L') {
    f = va_arg(factor, long double);
  } else {
    f = va_arg(factor, double);
  }
  parametrs->negative_num = 0;
  if (f < 0) {
    parametrs->negative_num = 1;
    f = -f;
    if (spec->accuracy != 0) {
      parametrs->len++;
    }
  }

  for (long long int n = (long long int)f; n != 0; n = n / 10, parametrs->len++)
    ;
  if ((long long int)f == 0) {
    parametrs->len = 1;
    if (parametrs->negative_num) parametrs->len++;
  }
  parametrs->len = parametrs->len + spec->accuracy;

  if ((unsigned long long int)f == 0) parametrs->integer = 1;
  conditions_for_float(spec, str, parametrs);
  parametrs->count = f_function(str, f, parametrs->count++, spec, end_programm);
  if (spec->minus && spec->width) {
    parametrs->count = insert_spaces(str, (spec->width - parametrs->len),
                                     parametrs->count, ' ');
  }
  parametrs->count--;
  return parametrs->count;
}

int check_condition_for_float(description *spec, options *parametrs) {
  int result = 0;
  if ((spec->plus && !spec->minus && !parametrs->negative_num) ||
      (spec->space && spec->accuracy && !spec->plus &&
       ((!spec->minus && parametrs->negative_num &&
         parametrs->len - spec->accuracy == 1) ||
        (spec->width && spec->minus && !parametrs->negative_num))) ||
      (spec->accuracy && spec->width && !spec->plus && !spec->space &&
       !parametrs->negative_num) ||
      (spec->plus && spec->minus && !spec->space &&
       ((!parametrs->negative_num && spec->accuracy == 0) ||
        (parametrs->negative_num && spec->accuracy) ||
        (!parametrs->negative_num && spec->width && spec->accuracy))) ||
      (spec->space && spec->accuracy && spec->width &&
       ((spec->plus && parametrs->negative_num) ||
        (spec->plus && spec->minus && !parametrs->negative_num) ||
        (spec->minus && parametrs->negative_num && !spec->plus)))) {
    result = 1;
  }
  return result;
}

void conditions_for_float(description *spec, char *str, options *parametrs) {
  if (check_condition_for_float(spec, parametrs)) {
    parametrs->len++;
  }
  if ((spec->accuracy == 0 &&
       ((parametrs->negative_num || (spec->plus && !spec->width)))) ||
      ((spec->space || (spec->plus && !parametrs->negative_num) ||
        (parametrs->negative_num && spec->width && !spec->plus)) &&
       spec->accuracy)) {
    parametrs->len++;
  }
  if (spec->space && spec->accuracy && parametrs->negative_num &&
      (spec->minus || spec->plus)) {
    parametrs->len--;
  }
  int check_space = 0;
  if (!spec->minus && spec->width) {
    long long int check_change_count = parametrs->count;
    parametrs->count = insert_spaces(str, (spec->width - parametrs->len),
                                     parametrs->count, ' ');
    if (check_change_count != parametrs->count) check_space = 1;
  }
  if (spec->minus && spec->plus && spec->space && spec->accuracy > 0)
    ;
  else {
    if (spec->space && !parametrs->negative_num &&
        ((spec->width - parametrs->len) > 0 ||
         (!spec->minus && !spec->plus && spec->accuracy >= 0))) {
      if (!check_space) {
        *(str + parametrs->count++) = ' ';
      }
    }
    if (spec->width > 0 && !spec->minus && !spec->plus && spec->accuracy &&
        !parametrs->negative_num && (spec->width - parametrs->len <= 0)) {
      if (check_space) {
        *(str + parametrs->count++) = ' ';
      }
    }
  }
  if (parametrs->negative_num) {
    *(str + parametrs->count++) = '-';
  } else if (spec->plus) {
    *(str + parametrs->count++) = '+';
  }
}

int processing_for_unsigned(char *str, va_list factor, description *spec,
                            options *parametrs) {
  parametrs->len = 0;
  parametrs->check_unsigned = 1;
  unsigned long int u_int = 0;
  unsigned long long int n = 0;
  if (spec->lenght == 'h') {
    u_int = va_arg(factor, unsigned int);
    parametrs->integer = u_int;
  } else if (spec->lenght == 'l') {
    u_int = va_arg(factor, unsigned long int);
    parametrs->integer = u_int;
  } else if (spec->lenght == 'U') {
    u_int = va_arg(factor, unsigned long long int);
    parametrs->integer = u_int;
  } else if (spec->lenght == 0) {
    u_int = va_arg(factor, unsigned int);
    parametrs->integer = u_int;
  }
  n = parametrs->integer;
  for (; n > 0; n = n / 10, parametrs->len++)
    ;
  parametrs->count = conditions_for_int(spec, str, parametrs);
  if (!(parametrs->integer == 0 && spec->check_point == 1)) {
    parametrs->count = d_function(str, parametrs->integer, parametrs->count);
  }  // главная функция для инта
  if (spec->minus && spec->width) {
    if (spec->accuracy > parametrs->len)
      parametrs->len += (spec->accuracy - parametrs->len);
    if (spec->accuracy == -2 && parametrs->integer == 0 && !spec->space)
      parametrs->len += 1;
    parametrs->count = insert_spaces(str, (spec->width - parametrs->len),
                                     parametrs->count, ' ');
  }
  parametrs->count--;
  return parametrs->count;
}

int processing_for_string(char *str, va_list factor, description *spec,
                          int count) {
  int len = 0;
  char *string_arg = NULL;
  string_arg = va_arg(factor, char *);
  if (string_arg == NULL) {
    len = 6;
  } else {
    len = s21_strlen(string_arg);
  }
  count = insert_a_line(spec, count, len, str, string_arg);
  return count;
}

int insert_a_line(description *spec, int count, int len, char *str,
                  char *string_arg) {
  char string_arg_tmp[BUF_SIZE] = "(null)";
  if (string_arg != NULL) {
    s21_strcpy(string_arg_tmp, string_arg);
  }
  if (spec->accuracy == -1) {
    spec->accuracy = 0;
  } else if (spec->accuracy == -2) {
    spec->accuracy = len;
  } else if (spec->accuracy >= len) {
    spec->accuracy = len;
  }
  string_arg_tmp[spec->accuracy] = '\0';
  if (!spec->minus && spec->width) {
    if (string_arg == NULL && spec->accuracy < 6) {
      count = insert_spaces(str, spec->width, count, ' ');
    } else {
      count = insert_spaces(str, (spec->width - spec->accuracy), count, ' ');
    }
  }
  if (string_arg == NULL && spec->accuracy >= 6) {
    for (int i = 0; string_arg_tmp[i]; i++) {
      *(str + count++) = string_arg_tmp[i];
    }
  } else if (string_arg != NULL) {
    for (int i = 0; string_arg_tmp[i]; i++) {
      *(str + count++) = string_arg_tmp[i];
    }
  }
  if (spec->minus && spec->width) {
    if (string_arg == NULL && spec->accuracy < 6) {
      count = insert_spaces(str, spec->width, count, ' ');
    } else {
      count = insert_spaces(str, (spec->width - spec->accuracy), count, ' ');
    }
  }
  count--;
  return count;
}

int processing_for_char(char *str, va_list factor, description *spec,
                        int count) {
  char char_arg = 0;
  char_arg = va_arg(factor, long int);
  if (!spec->minus && spec->width) {
    count = insert_spaces(str, (spec->width - 1), count, ' ');
  }
  *(str + count) = char_arg;
  if (spec->minus && spec->width) {
    count = insert_spaces(str, (spec->width - 1), count + 1, ' ') - 1;
  }
  return count;
}

int processing_for_percentage(char *str, description *spec, int count) {
  if (!spec->minus && spec->width) {
    count = insert_spaces(str, (spec->width - 1), count, ' ');
  }
  *(str + count) = '%';
  if (spec->minus && spec->width) {
    count = insert_spaces(str, (spec->width - 1), count, ' ');
  }
  return count;
}

// Функция вставляет указанный символ (как memset, но вставляет не с начала и
// возвращает счетчик)
int insert_spaces(char *str, int width, int count, int c) {
  for (int i = 0; i < width; i++) {
    *(str + count) = c;
    count++;
  }
  return count;
}

// Функция для получения числа из строки
char *get_a_number(char *str, int *number, flag_error *end_programm,
                   int width_with_zero, int *flag_zero) {
  *flag_zero = 0;
  // создается массив для точности
  char *buffer = (char *)malloc(1 * sizeof(char));
  if (buffer != NULL) {  // надо правильно обработать ошибку
    // в цикле помещаю точность в строку и расширяю реалоком, пока не встретится
    int j = 0;
    int size_buffer = 0;
    while ((*str != 'L' && *str != 'l' && *str != 'f' && *str != '.' &&
            *str != 's' && *str != 'c' && *str != 'd' && *str != '%' &&
            *str != 'h' && *str != 'u') &&
           end_programm->error != 1) {
      if (j == 0 && width_with_zero && *str == '0') *flag_zero = 1;
      j++;
      if (*str < '0' || *str > '9') {
        end_programm->error = 1;
        continue;
      }  // если после точки не цифра или f, то выдает ошибку
      *number = 0;
      buffer[size_buffer] = *str;
      size_buffer++;
      buffer = (char *)realloc(buffer, (size_buffer + 1) * sizeof(char));
      if (buffer == NULL) {
        end_programm->error = 1;
        break;
      }
      str++;
    }
    if (!end_programm->error) {
      *number = get_int_from_char(size_buffer, buffer, *number);
    }
    free(buffer);
  } else {
    end_programm->error = 1;
  }
  return str;
}

int get_int_from_char(int size_buffer, char *buffer, int number) {
  int discharge = 10;
  // в цикле определяю точность из строки
  for (int i = size_buffer - 1; i > -1; i--) {
    int value = transfer_char(buffer[i]);
    if (i != size_buffer - 1) {
      value *= discharge;
      discharge *= 10;
    }
    number += value;
  }
  return number;
}

// перевод инт в чар
char transfer_int(int a) {
  char c = 48 + a;
  return c;
}

// перевод чар в инт
int transfer_char(char b) {
  int c = b - 48;
  return c;
}

// Функция для обработки целых чисел (int)
int d_function(char *str, unsigned long long int d, int count) {
  unsigned long long int ten = 10;
  // условие для многозначных чисел
  if (d / ten >= 1) {
    while (d / ten > 9) {
      ten *= 10;
    }
    long long int full_num = d / ten;
    d = d - full_num * ten;
    *(str + count++) = transfer_int(full_num);
    while (ten != 10) {
      ten /= 10;
      full_num = d / ten;
      d = d - full_num * ten;
      *(str + count++) = transfer_int(full_num);
    }
    full_num = d;
    *(str + count++) = transfer_int(full_num);
  } else {
    *(str + count++) = transfer_int(d);
  }
  return count;
}

// Функция для обработки чисел с плав. точкой (float)
int f_function(char *str, long double f, int count, description *spec,
               flag_error *end_programm) {
  options parametrs = {0};
  parametrs.checkpoint_count = count;
  parametrs.checkpoint_f = f;
  if (f != 0) {
    function_for_long_f(f, end_programm);
  }
  parametrs.integer = (unsigned long long int)f;
  if (spec->accuracy == 0) {
    parametrs.integer = roundl(f);
  }
  count = d_function(str, parametrs.integer, count);
  if (spec->accuracy != 0) {
    if (f != (unsigned long long int)f) {
      unsigned long long int buf_f = (unsigned long long int)f;
      f = f - buf_f;
    } else
      f = 0;
    if (spec->accuracy) {
      parametrs.count = count;
      count = fractional_part_of_a_number(str, f, &parametrs, spec);
    }
  }
  return count;
}

int conditions_for_int(description *spec, char *str, options *parametrs) {
  parametrs->combined_width = (spec->accuracy - parametrs->len);
  if (!spec->minus && spec->width > parametrs->len) {
    if (spec->accuracy > parametrs->len) {
      parametrs->count = accuracy_bigger_len_function(spec, str, parametrs);
    } else {
      parametrs->combined_width = (spec->width - parametrs->len - 1);
      if (parametrs->negative_num) {
        parametrs->count = negative_num_function(spec, str, parametrs);
      } else if (spec->plus && !parametrs->check_unsigned) {
        parametrs->count =
            insert_spaces_with_minus_or_plus(str, parametrs, ' ', '+');
      } else {
        deep_conditions_for_int(spec, parametrs, str);
      }
    }
  } else if (spec->space) {
    parametrs->count = space_function(spec, str, parametrs);
  } else if (spec->accuracy > parametrs->len) {
    parametrs->count = accuracy_bigger_len_function(spec, str, parametrs);
  } else if (parametrs->negative_num) {
    *(str + parametrs->count++) = '-';
  } else if (spec->plus && !parametrs->check_unsigned) {
    *(str + parametrs->count++) = '+';
  }
  return parametrs->count;
}

void deep_conditions_for_int(description *spec, options *parametrs, char *str) {
  if (parametrs->integer == 0 && spec->check_point == 0) spec->width--;
  parametrs->combined_width = (spec->width - parametrs->len);
  if (spec->check_point || parametrs->integer == 0) {
    parametrs->count =
        insert_spaces(str, parametrs->combined_width, parametrs->count, ' ');
  } else if (!spec->check_point) {
    parametrs->count =
        insert_spaces(str, parametrs->combined_width, parametrs->count, ' ');
  } else {
    // проверить для чего это условие (возможны ошибки)
    parametrs->count =
        insert_spaces(str, parametrs->combined_width, parametrs->count, '0');
  }
}

int insert_spaces_with_minus_or_plus(char *str, options *parametrs, char c,
                                     char sign) {
  parametrs->count =
      insert_spaces(str, parametrs->combined_width, parametrs->count, c);
  *(str + parametrs->count++) = sign;
  return parametrs->count;
}

int negative_num_function(description *spec, char *str, options *parametrs) {
  if (spec->check_point || parametrs->integer == 0 || !spec->check_zero ||
      (spec->accuracy > parametrs->len)) {
    parametrs->count =
        insert_spaces_with_minus_or_plus(str, parametrs, ' ', '-');
  } else {
    *(str + parametrs->count++) = '-';
    parametrs->count =
        insert_spaces(str, parametrs->combined_width, parametrs->count, '0');
  }
  return parametrs->count;
}

int accuracy_bigger_len_function(description *spec, char *str,
                                 options *parametrs) {
  if (!spec->minus && spec->width > parametrs->len) {
    parametrs->combined_width =
        spec->width - parametrs->len - (spec->accuracy - parametrs->len);
    if (parametrs->negative_num) {
      parametrs->combined_width--;
      parametrs->count = negative_num_function(spec, str, parametrs);
    } else if (spec->plus && !parametrs->check_unsigned) {
      parametrs->combined_width--;
      parametrs->count =
          insert_spaces_with_minus_or_plus(str, parametrs, ' ', '+');
    } else {
      parametrs->count =
          insert_spaces(str, parametrs->combined_width, parametrs->count, ' ');
    }
    parametrs->count = insert_spaces(str, (spec->accuracy - parametrs->len),
                                     parametrs->count, '0');
  } else if (!spec->space) {
    if (parametrs->negative_num) {
      *(str + parametrs->count++) = '-';
    } else if (spec->plus && !parametrs->check_unsigned) {
      *(str + parametrs->count++) = '+';
    }
    parametrs->count =
        insert_spaces(str, parametrs->combined_width, parametrs->count, '0');
  }
  return parametrs->count;
}

int space_function(description *spec, char *str, options *parametrs) {
  if (!parametrs->check_unsigned) {
    if (!parametrs->negative_num)
      *(str + parametrs->count++) = ' ';
    else
      *(str + parametrs->count++) = '-';
  }
  if (spec->accuracy > parametrs->len) {
    parametrs->count =
        insert_spaces(str, parametrs->combined_width, parametrs->count, '0');
  }
  return parametrs->count;
}

void function_for_long_f(long double f, flag_error *end_programm) {
  long double separeted_f = f;
  int count_for_separeted_f = 0;
  while (floorl(separeted_f / 10) != 0) {
    separeted_f = separeted_f / 10;
    count_for_separeted_f++;
  }
  if (count_for_separeted_f > 30) {
    end_programm->error = 1;
  }
}

int fractional_part_of_a_number(char *str, long double f, options *parametrs,
                                description *spec) {
  *(str + parametrs->count++) = '.';

  if (f == 0) {
    for (int i = 0; i < spec->accuracy; i++) {
      parametrs->count = d_function(str, 0, parametrs->count);
    }
  } else {
    if (f < 0) f *= -1;
    int count_ten = 0;

    while ((unsigned long long int)f == 0 && count_ten < spec->accuracy) {
      f *= 10;
      count_ten++;
    }
    for (int i = 0; i < count_ten - 1; i++) {
      parametrs->count = d_function(str, 0, parametrs->count);
    }

    while (count_ten < spec->accuracy) {
      f *= 10;
      count_ten++;
    }
    f = roundl(f);
    parametrs->count = rounding_check(str, f, parametrs, spec);
  }
  return parametrs->count;
}

int rounding_check(char *str, long double f, options *parametrs,
                   description *spec) {
  unsigned long long int buf = (unsigned long long int)f;
  unsigned long long int buf_f = buf;
  int check_okruglenie = 0;
  int fullnum_check = 0;
  while (buf_f != 1 && buf_f != 0) {
    buf_f = buf_f / 10;
    check_okruglenie++;

    if (buf_f == 1 && check_okruglenie == spec->accuracy) {
      fullnum_check = 1;
    }
  }
  if (fullnum_check) {
    parametrs->count = parametrs->checkpoint_count;
    f = parametrs->checkpoint_f;
    parametrs->integer = (unsigned long long int)f + 1;
    parametrs->count = d_function(str, parametrs->integer, parametrs->count);

    if (spec->accuracy) {
      *(str + parametrs->count++) = '.';
      for (int i = 0; i < spec->accuracy; i++) {
        parametrs->count = d_function(str, 0, parametrs->count);
      }
    }
  }
  if (fullnum_check == 0)
    parametrs->count = d_function(str, buf, parametrs->count);
  return parametrs->count;
}