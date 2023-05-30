APP = example
OBJ_DIR = build
OBJ = example.o xnr_io.o xnr_line.o xnr_history.o xnr_key.o xnr_var.o
OBJ_ADD_DIR := $(addprefix $(OBJ_DIR)/, $(OBJ))

all : mkbuild $(OBJ)
	gcc -g $(OBJ_ADD_DIR)  -o $(OBJ_DIR)/$(APP)

%.o : %.c
	gcc -c -Wall -Werror -o $(OBJ_DIR)/$@ $<

clean:
	rm -rf $(OBJ_DIR)

mkbuild:
	mkdir -p $(OBJ_DIR)

check: mkbuild
	cppcheck --enable=all --inconclusive --std=posix ./  2>&1 | tee build/check.txt

.PHONY: clean, mkbuild