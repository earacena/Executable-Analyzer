#####################################
#  Makefile for Executable-Analyzer #
#  Author: Emanuel Aracena          #
#  Target platform(s): Linux 64-bit #
#####################################


CC   = g++
HEAD = 
OBJ  = main.o Tracer.o Logger.o
CLEAN= rm -rf 
EXEC = dec


default: dec

%.o: %.cpp $(HEAD)
	$(CC) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	$(CLEAN) $(OBJ)
	$(CLEAN) $(EXEC)
