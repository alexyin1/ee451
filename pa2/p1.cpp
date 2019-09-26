#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define BILLION 1E9

struct column_block{
   int thread_id;
   int n;
   int blockwidth;
   int startcol;
   double **A;
   double **B;
   double **C;
};



void *col_matmul(void *block_args){
	struct column_block * my_data;
	my_data = (struct column_block *) block_args;
	int thread_id = my_data->thread_id;
 	int n = my_data->n;
 	int blockwidth = my_data->blockwidth;
 	int startcol = my_data->startcol;
 	double **A = my_data->A;
 	double **B = my_data->B;
 	double **C = my_data->C;

 	for(int i=0; i<n; i++){
		for(int j=startcol; j<startcol+blockwidth; j++){
			for(int k=0; k<n; k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
  	//printf("Thread %d now complete\n", thread_id);
	pthread_exit(NULL);
	// return;
}

void display_results(int n, struct timespec start, struct timespec stop, double** C){
	double time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

	printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", (long)2*n*n*n, time, 1/time/1e6*2*n*n*n);		
	printf("C[100][100]=%f\n", C[100][100]); //C[100][100]

	double accum = ( stop.tv_sec - start.tv_sec )
	  + ( stop.tv_nsec - start.tv_nsec )
	  / BILLION;
	printf( "%lf seconds \n", accum );
	return;
}

void naive_matmul(int n, double**A, double**B, double** C){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			for(int k=0; k<n; k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return;
}

void reset(int n, double**A, double**B, double** C){
	for (int i=0; i<n; i++){
		for(int j=0; j< n; j++){
			A[i][j]=i;
			B[i][j]=i+j;
			C[i][j]=0;			
		}
	}
	return;
}



int main(int argc, char *argv[]) {
	int NUM_THREADS = atoi(argv[1]);
	struct column_block column_block_array[NUM_THREADS];
	struct timespec start, stop;
	int n = 4096;
	//allocate memory
	double ** A = new double*[n];
	double ** B = new double*[n];
	double ** C_naive = new double*[n];
	double ** C_parallel = new double*[n];
	for (int i=0; i<n; i++){
		A[i] = new double[n];
		B[i] = new double[n];
		C_naive[i] = new double[n];
		C_parallel[i] = new double[n];
	}
	reset(n, A, B, C_naive);
	reset(n, A, B, C_parallel);
//start timer
// 	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
// 	naive_matmul(n, A, B, C_naive);
// //end timer 
// 	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}	
// 	display_results(n, start, stop, C_naive);


	pthread_t  threads[NUM_THREADS];
	int rc, i;
	int blockwidth = n/NUM_THREADS;
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}

	for(int i=0; i<NUM_THREADS; i++) {	  
		column_block_array[i].thread_id = i;
		column_block_array[i].n = n;
		column_block_array[i].blockwidth = blockwidth;
		column_block_array[i].startcol = blockwidth*i;
		column_block_array[i].A = A;
		column_block_array[i].B = B;
		column_block_array[i].C = C_parallel;
		rc = pthread_create(&threads[i], NULL, col_matmul, (void *) &column_block_array[i]);
		if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}
	}
	
	for(int j=0; j<NUM_THREADS; j++){
        pthread_join(threads[j], NULL);
    }

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}	
	display_results(n, start, stop, C_parallel);
	// for(int i=0; i<n; i++){
	// 	for(int j=0; j<n; j++){
	// 		if(	C_parallel[i][j] != C_naive[i][j]){
	// 			printf("ERROR at index %d,%d, %f not equal to %f", i, j, C_parallel[i][j], C_naive[i][j]);
	// 		}
	// 	}
	// }

	//deallocate memory
	for (int i=0; i<n; i++){
		delete A[i];
		delete B[i];
		delete C_naive[i];
		delete C_parallel[i];
	}
	delete [] A;
	delete [] B;
	delete [] C_naive;
	delete [] C_parallel;

	pthread_exit(NULL);
}



