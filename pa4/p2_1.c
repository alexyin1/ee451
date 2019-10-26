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
	//all threads  read whole array
	int size = 64;
	int nums[size];
	FILE *myfile;
	myfile = fopen("number.txt", "r");
	for(int i = 0; i < size; i++){
		fscanf(myfile,"%d",&nums[i]);	
	}


	// printf("world_size%d", world_size);
	if (world_rank == 0) {
	    int a; 	    
	    int parts[4];
	    parts[0] = 0;
	    for(a=0; a < 16; a++){
	    	parts[0] += nums[a];
	    }
	    MPI_Recv(&parts[1], 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
	    MPI_Recv(&parts[2], 1, MPI_INT, 2, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
		MPI_Recv(&parts[3], 1, MPI_INT, 3, 0, MPI_COMM_WORLD,
	             MPI_STATUS_IGNORE);
	    for(int k=1; k<4; k++){
	    	parts[0] += parts[k];
	    }
	    printf("Process 0: Msg = %d Done!\n", parts[0]);
	} 
	else if (world_rank == 1) {
	    int b; int sumb = 0;
	    for(b=16; b < 32; b++){
	    	sumb += nums[b];
	    }
	    // printf("Process 1: Msg = %d Done!\n", sumb);
	    MPI_Send(&sumb, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else if (world_rank == 2) {
	  	int c; int sumc = 0;
	    for(c=32; c < 48; c++){
	    	sumc += nums[c];
	    }
	    // printf("Process 2: Msg = %d Done!\n", sumc);
	    MPI_Send(&sumc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else if (world_rank == 3) {
		int d; int sumd = 0;
	    for(d=48; d < 64; d++){
	    	sumd += nums[d];
	    }
	    // printf("Process 3: Msg = %d Done!\n", sumd);
	    MPI_Send(&sumd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);	
	}
	MPI_Finalize();
}
