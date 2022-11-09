GCC = gcc -Wall -Werror -Wextra -std=c11


all: s21_grep

s21_grep: Makefile s21_grep.h s21_grep.c
	chmod 777 s21_grep.c
	chmod 777 s21_grep.h
	$(GCC) s21_grep.c -c -o s21_grep.o 
	$(GCC) s21_grep.o -o s21_grep

check: clean all
	cp ../../materials/linters/.clang-format ../../src/.clang-format
	clang-format -style=Google -n s21_grep.c s21_grep.h
	rm ../.clang-format
	
test: clean all
	chmod 777 test.sh
	bash test.sh
	chmod 777 test_func_grep.sh
	bash test_func_grep.sh
	make clean

leaks: clean all
	chmod 777 test_leak_grep.sh
	sh test_leak_grep.sh

valgrind: clean all
	chmod 777 test_ERROR_VALGRIND.sh
	bash test_ERROR_VALGRIND.sh

rebuild: clean all

clean:
	rm -rf s21_grep s21_grep.dSYM *.o