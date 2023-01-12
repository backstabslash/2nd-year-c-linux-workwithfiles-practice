SRCDIR =../src
VPATH=$(SRCDIR)
TARGET = prog
OBJ = main.o
.PHONY: clean test
$(TARGET): $(OBJ)
	gcc -o $@ $^
$(OBJ): 

clean:
	rm -f $(TARGET) $(OBJ)

