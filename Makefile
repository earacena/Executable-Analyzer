CC   = gcc
HEAD = 
OBJ  = dec.o
CLEAN= rm -rf 
EXEC = dec


default: dec

%.o: %.c $(HEAD)
	$(CC) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	$(CLEAN) $(OBJ)
	$(CLEAN) $(EXEC)
