CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11

all: clean s21_cat

s21_cat : s21_cat.o
	$(CC) s21_cat.o -o s21_cat

s21_cat.o : s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) $< -o $@

test:
	sh tests_cat.sh

clean:
	rm -rf  *.o *.a *.so s21_cat

style:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -n s21_cat.c
	clang-format -n s21_cat.h
	rm .clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c
	leaks -quiet -atExit -- ./s21_cat -b ./tests/test_1_cat.txt