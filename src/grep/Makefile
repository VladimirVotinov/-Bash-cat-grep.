CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11

all: clean s21_grep

s21_grep: s21_grep.o
	$(CC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) $< -o $@

style:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -n s21_grep.c
	clang-format -n s21_grep.h
	rm .clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c
	leaks -quiet -atExit -- ./s21_grep -i "lcs" ./tests/test_1_grep.txt
test:
	sh tests_grep.sh
clean:
	rm -rf  *.o *.a *.so s21_grep