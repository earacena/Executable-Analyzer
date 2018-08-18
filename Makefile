CC   = gcc
HEAD = 
OBJ  = dec.o
CLEAN= shred -zun 10
EXEC = dec


default: dec

%.o: %.c $(HEAD)
	$(CC) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	$(CLEAN) $(OBJ)
	$(CLEAN) $(EXEC)
