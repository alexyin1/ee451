MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
MPI_LINK_FLAGS = $(shell mpicc --showme:link)

all: p1 p2_1 p2_2 p2_3

p1: p1.c 
		$(CC) $(MPI_COMPILE_FLAGS) p1.c $(MPI_LINK_FLAGS) -o p1

p2_1: p2_1.c 
		$(CC) $(MPI_COMPILE_FLAGS) p2_1.c $(MPI_LINK_FLAGS) -o p2_1 

p2_2: p2_2.c 
		$(CC) $(MPI_COMPILE_FLAGS) p2_2.c $(MPI_LINK_FLAGS) -o p2_2 

p2_3: p2_3.c 
		$(CC) $(MPI_COMPILE_FLAGS) p2_3.c $(MPI_LINK_FLAGS) -o p2_3 
