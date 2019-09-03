#include <time.h>
#include <stdio.h>
//#include <stdlib.h>
#define BILLION 1E9

void display_results(int n, struct timespec start, struct timespec stop, double** C){
	double time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

	printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", (long)2*n*n*n, time, 1/time/1e6*2*n*n*n);		
	printf("C[100][100]=%f\n", C[100][100]);

	double accum = ( stop.tv_sec - start.tv_sec )
	  + ( stop.tv_nsec - start.tv_nsec )
	  / BILLION;
	printf( "%lf seconds \n", accum );
	return;
}

void naive_matmul(int n, double**A, double**B, double** C){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			C[i][j] = A[i][j] * B[j][i];   
		}
	}
	return;
}

void block_matmul(int n, double**A, double**B, double** C, int block_size){
	for(int i=0; i<n; i+=block_size){
		for(int j=0; j<n; j+=block_size){
			for(int row=0 ;row<block_size; row++){
				for(int col=0; col<block_size; col++){
					C[i+row][j+col] += A[i+row][j+col] * B[j+col][i+row];
				}
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


int main(){
	struct timespec start, stop;
	int n = 4096;
	//allocate memory
	double ** A = new double*[n];
	double ** B = new double*[n];
	double ** C = new double*[n];
	for (int i=0; i<n; i++){
		A[i] = new double[n];
		B[i] = new double[n];
		C[i] = new double[n];
	}

	reset(n, A, B, C);
//start timer
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	naive_matmul(n, A, B, C);
//end timer 
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}	
	display_results(n, start, stop, C);


	int block_size = 16;
	reset(n, A, B, C);
//start timer
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	block_matmul(n, A, B, C, block_size);
//end timer 
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}	
	display_results(n, start, stop, C);



	//deallocate memory
	for (int i=0; i<n; i++){
		delete A[i];
		delete B[i];
		delete C[i];
	}
	delete [] A;
	delete [] B;
	delete [] C;
}
