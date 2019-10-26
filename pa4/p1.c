#include <mpi.h>
#include <stdio.h>
// Find out rank, size
int main(int argc, char *argv[]){
	int world_rank;
	int world_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int number;
	// printf("world_size%d", world_size);
	if (world_rank == 0) {
	    number = 451;
	    printf("Initially Process 0: Msg = %d \n", number);
	    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD	);
	    MPI_Recv(&number, 1, MPI_INT, 3, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
	    printf("Process 0: Msg = %d Done!\n", number);
	} 
	else if (world_rank == 1) {
	    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
		number++;
	    printf("Process 1: Msg = %d \n", number);
	    MPI_Send(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	}
	else if (world_rank == 2) {
	    MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
		number++;
	    printf("Process 2: Msg = %d \n", number);
	    MPI_Send(&number, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
	}
	else if (world_rank == 3) {
	    MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
		number++;
	    printf("Process 3: Msg = %d \n", number);
	    MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
