#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Find out rank, size
int main(int argc, char *argv[]){
	int world_rank;
	int world_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int number;
	//thread 0  read whole array
	int size = 64;
	int nums[size];
	if(world_rank == 0){
		FILE *myfile;
		myfile = fopen("number.txt", "r");
		for(int i = 0; i < size; i++){
			fscanf(myfile,"%d",&nums[i]);	
		}
	}

	int sub_size = 16;
	int *sub_nums = malloc(sizeof(int) * sub_size);

	MPI_Scatter(&nums, sub_size, MPI_INT, sub_nums, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    int a; 	    
    int localsum = 0;

    for(a=0; a < sub_size; a++){
		localsum += sub_nums[a];
	} 
    // printf("localsum %d = %d\n", world_rank, localsum);

	int* localsums = NULL;

	if(world_rank == 0){
		localsums = malloc(sizeof(int) * world_size);
	}
											   //number of elements per reception, not total elements received
	MPI_Gather(&localsum, 1, MPI_INT, localsums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
    	int globalsum = 0;
    	for(int i=0; i<world_size; i++){
    		globalsum += localsums[i];
		    printf("localsums[%d] = %d\n", i, localsums[i]);
    	}
	    printf("Process 0: Msg = %d Done!\n", globalsum);
	} 
	MPI_Finalize();
}
