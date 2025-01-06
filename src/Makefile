CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
NCURSES_FLAGS = -lncurses
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
FORMAT = clang-format

NAME_PROGRAM = program/BrickGame
FILE_SCORE = program/score_list.txt
DIR_BUILD = build

LIB_TETRIS = brick_game/tetris/tetris.a

FILES_ESSENTIAL = essential/*.c
FILES_INTERFACE = gui/cli/*.c 
FILES_MENU = brick_game/menu/*.c
FILES_TETRIS = brick_game/tetris/*.c 
FILES_TETRIS_MACHINE = brick_game/FSMs/tetris_machine/*.c

NAME_TEST = tetris_test
LIB_TETRIS_TEST = tests/$(NAME_TEST).a
PROGRAM_TETRIS_TEST = tests/$(NAME_TEST)
FILE_TETRIS_TEST = tests/$(NAME_TEST).c
OBJ_TETRIS_TEST = $(DIR_BUILD)/$(NAME_TEST).o


GCNO_TEST_FILES = $(DIR_BUILD)/*.gcno
OBJ_FILES = $(DIR_BUILD)/*.o
OBJ_ESSENTIAL = build/essential.o
GCOV_FOLDER = tests/gcov_report

ifeq ($(shell uname -s), Linux)
	CHECK_FLAGS = -lcheck -lm -lrt -lpthread -lsubunit
	MEM_LEAKAGE = sleep 2; CK_FORK=no valgrind --leak-check=full --track-origins=yes -s ./$(PROGRAM_TETRIS_TEST)
else
	CHECK_FLAGS = -lcheck
	MEM_LEAKAGE = sleep 2; leaks -atExit -- ./$(PROGRAM_TETRIS_TEST) | grep LEAK:
endif

all : install
	./$(NAME_PROGRAM)

create_build :
	@if [ ! -d "$(DIR_BUILD)" ]; then \
		mkdir $(DIR_BUILD); \
	fi

delete_build :
	@if [ -d "$(DIR_BUILD)" ]; then \
		rm -R $(DIR_BUILD); \
	fi

clean_build :
	@if [ -d "$(DIR_BUILD)" ]; then \
		rm -rf $(DIR_BUILD)/*; \
	fi

tetris.a : create_build
	$(CC) $(CFLAGS) $(NCURSES_FLAGS) -c $(FILES_TETRIS)
	mv *.o $(DIR_BUILD)
	ar rc $(LIB_TETRIS) $(OBJ_FILES)
	ranlib $(LIB_TETRIS)

install : clean_build tetris.a
	$(CC) $(CFLAGS) $(NCURSES_FLAGS) -c $(FILES_INTERFACE)
	$(CC) $(CFLAGS) -c $(FILES_ESSENTIAL)
	$(CC) $(CFLAGS) -c $(FILES_MENU)
	$(CC) $(CFLAGS) -c $(FILES_TETRIS_MACHINE)
	mv *.o $(DIR_BUILD)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(NAME_PROGRAM) $(OBJ_FILES) $(LIB_TETRIS) $(NCURSES_FLAGS)
	touch $(FILE_SCORE)
	echo "0\n0\n" > $(FILE_SCORE)

uninstall : clean
	rm -rf $(FILE_SCORE) $(NAME_PROGRAM)

clean : delete_build delete_dvi delete_dist
	rm -rf $(PROGRAM_TETRIS_TEST) $(GCOV_FOLDER)  $(shell find . -name '*.gcda') $(shell find . -name '*.gcno') $(shell find . -name '*.o') $(shell find . -name '*.log') $(shell find . -name '*.a')

dvi : doxygen_latex
	cd docs/latex && $(MAKE) && cd ../..
	open docs/html/index.html

doxygen_latex :
	doxygen Doxyfile

delete_dvi :
	@if [ -d "docs" ]; then \
		rm -rf docs; \
	fi

dist :
	tar -czf BrickGame-$(shell date +%d-%m-%Y).tar.gz brick_game gui essential tests Makefile Doxyfile

delete_dist:
	@rm -f BrickGame-*.tar.gz

tetris_test.a : tetris.a
	$(CC) $(CFLAGS) $(NCURSES_FLAGS) $(GCOV_FLAGS) -c $(FILE_TETRIS_TEST)
	mv *.o $(DIR_BUILD)
	ar rc $(LIB_TETRIS_TEST) $(OBJ_TETRIS_TEST)
	ranlib $(LIB_TETRIS_TEST)
	mv *.gcno $(DIR_BUILD)

test : clean_build tetris_test.a
	$(CC) $(CFLAGS) -c $(FILES_ESSENTIAL)
	mv *.o $(DIR_BUILD)
	$(CC) $(CFLAGS) $(NCURSES_FLAGS) $(GCOV_FLAGS) -o $(PROGRAM_TETRIS_TEST) $(LIB_TETRIS) $(LIB_TETRIS_TEST) $(FILES_TETRIS) $(CHECK_FLAGS) $(OBJ_ESSENTIAL)
	./$(PROGRAM_TETRIS_TEST)
	mv tests/*.gcda tests/*.gcno $(DIR_BUILD) 

gcov_report : test
	$(foreach file, $(wildcard $(GCNO_TEST_FILES)), gcov $(file);)
	lcov --capture --directory . --output-file html.info --no-external
	genhtml -o $(GCOV_FOLDER) html.info
	echo "Code coverage report generated in gcov_report directory."
	mv *.info $(DIR_BUILD)
	open $(GCOV_FOLDER)/index.html
	mv *.gcov $(DIR_BUILD)

checking : test
	$(FORMAT) -n -style=Google $(shell find . -name '*.c') $(shell find . -name '*.h')
	$(MEM_LEAKAGE)
	sleep 2; cppcheck --enable=all --suppress=missingIncludeSystem $(shell find . -name '*.c')
	rm -R $(DIR_BUILD)
	rm -rf gcov_report *.a *.gcda $(NAME_TEST)
	$(MAKE) clean

style :
	$(FORMAT) -i -style=Google $(shell find . -name '*.c') $(shell find . -name '*.h')