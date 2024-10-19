PROJECT_NAME = s21_matrix
FLAGS = -Wall -Wextra -Werror -std=c11
CHECKFLAG = -lcheck
UNAME= $(shell uname -s)
ifeq ($(UNAME), Linux)
	LDFLAGS += -lm -lsubunit -lpthread -lrt
	CFLAGS += -lm
endif


all: clean ${PROJECT_NAME}.a gcov_report

$(PROJECT_NAME).a:
	gcc -c $(FLAGS) $(PROJECT_NAME).c
	ar rcs $(PROJECT_NAME).a $(PROJECT_NAME).o
	ranlib $(PROJECT_NAME).a

test: clean $(PROJECT_NAME).a
	gcc $(FLAGS) $(CHECKFLAG) $(PROJECT_NAME).a s21_test.c -o s21_test
	./s21_test

gcov_report: 
	gcc -c $(FLAGS) s21_test.c
	gcc $(FLAGS) --coverage $(CHECKFLAG) $(PROJECT_NAME).c s21_test.o -o s21_test
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
ifeq ($(UNAME), Linux)
	xdg-open report/index.html
else
	open report/index.html
endif

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_report.txt ./s21_test

style_check:
# 	cppcheck --enable=all --suppress=missingIncludeSystem s21_*.c s21_*.h
#	clang-format -style=Google -i *.c *.h
	cp ../materials/linters/.clang-format .
	clang-format -n -style=Google *.c *.h
	rm -f .clang-format

leak_check: test
ifeq ($(UNAME), Linux)
	CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all --tool=memcheck --trace-children=yes --track-origins=yes ./s21_test
else
	CK_FORK=no leaks --atExit -- ./s21_test 
endif

clean: 
	rm -rf *.o *.a *.gcno *.gcda *.info s21_test report *.out

format:
	clang-format -style=google -i *.c *.h 

