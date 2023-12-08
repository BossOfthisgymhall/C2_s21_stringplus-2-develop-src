CC = gcc
FLAGS = -Wall -Wextra -Werror -pedantic -std=c11
OS = $(shell uname -s)

M = something new
BRANCH = $(shell git branch --show-current)

GCOV_FLAGS=-fprofile-arcs -ftest-coverage
TEST_SRC=$(wildcard s21_unit_test.c)
CFILES=$(wildcard s21_string_functions/s21_*.c)


all: s21_string.a test

clean:
	rm -rf *.a
	rm -rf s21_test
	rm -rf report

s21_string.a:
	$(CC) $(FLAGS) -c s21_string_functions/*.c s21_c_sharp_func/*.c
	ar rcs s21_string.a *.o
	ranlib s21_string.a
	rm *.o

test:
ifeq ($(OS), Linux)
	gcc -Wall -Werror -Wextra s21_unit_test.c -L. s21_string.a -lcheck -lm -lpthread -lsubunit -o s21_test
else 
	gcc -Wall -Werror -Wextra s21_unit_test.c  -L. s21_string.a -lcheck -lpthread -o s21_test
endif
	./s21_test

gcov_report:
ifeq ($(OS), Linux)
	$(CC) $(CFLAGS) --coverage s21_unit_test.c s21_string_functions/*.c s21_c_sharp_func/*.c -o gcov -lcheck -lm -lsubunit
else
	$(CC) $(CFLAGS) --coverage s21_unit_test.c s21_string_functions/*.c s21_c_sharp_func/*.c -o gcov -lcheck
endif
	chmod +x gcov
	./gcov 
	lcov -t "s21_test" -o fizzbuzz.info -c -d .
	genhtml -o report fizzbuzz.info
ifeq ($(OS), Linux)
else
	open report/index.html
endif
	rm *.gcda *.gcno *.info gcov

rebuild: clean s21_string.a 

check_style:
	cp ../materials/linters/.clang-format .
	clang-format -n s21_string_functions/*.c *.h s21_c_sharp_func/*.c *.c 

correct_style:
	cp ../materials/linters/.clang-format .
	clang-format -i s21_string_functions/*.c *.h

gitpush:
	git add .
	git commit -m "$(BRANCH): $(M)"
	git push origin $(BRANCH)

gitpull:
	git pull origin $(BRANCH)

# Отменяет неиндексированные изменение (командой git add)
gitback:
	git stash save --keep-index
