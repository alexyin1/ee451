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

    MPI_Bcast(&nums, 64, MPI_INT, 0, MPI_COMM_WORLD);

    int a; 	    
    int localsum = 0;
	if (world_rank == 0) {
	    for(a=0; a < 16; a++){
			localsum += nums[a];
	    }
	} 
	else if (world_rank == 1) {
	    for(a=16; a < 32; a++){
	    	localsum += nums[a];
	    }
	}
	else if (world_rank == 2) {
	    for(a=32; a < 48; a++){
	    	localsum += nums[a];
	    }
	}
	else if (world_rank == 3) {
	    for(a=48; a < 64; a++){
	    	localsum += nums[a];
	    }
	}
	int globalsum;
    MPI_Reduce(&localsum, &globalsum, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
	    printf("Process 0: Msg = %d Done!\n", globalsum);
	} 
	MPI_Finalize();
}
