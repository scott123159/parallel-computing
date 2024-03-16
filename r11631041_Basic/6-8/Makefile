MPICC = mpicc
FLAGS = -lm -std=c99

FILE_NAME = exer06_08
SRC = $(FILE_NAME).c
OBJ = $(FILE_NAME).o 

$(FILE_NAME): $(OBJ)
	$(MPICC) $(OBJ) -o $(FILE_NAME)

$(OBJ): $(SRC)
	$(MPICC) $(FLAGS) -c $(SRC)

clean:
	rm *.o assignment.* $(FILE_NAME)

run:
	mpirun ./$(FILE_NAME)
