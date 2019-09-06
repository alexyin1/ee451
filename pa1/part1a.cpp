#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define BILLION 1E9

void display_results(int n, struct timespec start, struct timespec stop, double** C){
	double time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

	printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", (long)2*n*n*n, time, 1/time/1e6*2*n*n*n);		
	printf("C[1][1]=%f\n", C[1][1]); //C[100][100]

	double accum = ( stop.tv_sec - start.tv_sec )
	  + ( stop.tv_nsec - start.tv_nsec )
	  / BILLION;
	printf( "%lf seconds \n", accum );
	return;
}

void naive_matmul(int n, double**A, double**B, double** C){
	//double sum = 0;
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			//sum = 0;
			for(int k=0; k<n; k++){
				C[i][j] += A[i][k] * B[k][j];
				//sum += A[i][k] * B[k][j];   
			}
			//C[i][j] = sum;
		}
//		printf ("n=%i, i=%i, j=%i \n", n, i, 0);
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


int main(int argc, char* argv[]){
	struct timespec start, stop;
	int n = atoi(argv[1]); //4096;
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
