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
	double sum = 0;
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			sum = 0;
			for(int k=0; k<n; k++){
				sum += A[i][k] * B[k][j];   
			}
			C[i][j] = sum;
		}
//		printf ("n=%i, i=%i, j=%i \n", n, i, 0);
	}
	return;
}

void block_matmul(int n, double**A, double**B, double** C, int block_size){
	printf("before: %d \n", n);
	// double block[block_size][block_size];
	// //clear block for each index C(i,j)
	// for(int i=0; i<block_size; i++){				
	// 	for(int j=0; j<block_size; j++){
	// 		block[i][j] = 0;
	// 	}
	// }
	double **block = new double*[block_size];
	for(int i=0; i<block_size; i++){				
		block[i] = new double[block_size];
		for(int j=0; j<block_size; j++){
			block[i][j] = 0;
		}
	}

	for(int i=0; i<n; i+=block_size){			//rows
		for(int j=0; j<n; j+=block_size){		//cols
			for(int k=0; k<n; k+=block_size){   
				//naive_matmul(block_size, , double **B, block_size)
				// for(int row=0; row<block_size; row++){
				// 	for(int col=0; col<block_size; col++){
				// 		for(int kk=0; kk<block_size; kk++){
				// 			block[row][col] += A[i+row][k+kk] * B[k+kk][j+col];		
				// 		}
				// 	}
				// }
			}
			//set C[i:i+blocksize, j:j+blocksize] = block and
			//clear block for each index C(i,j)
			for(int row=0; i<block_size; row++){	
				for(int col=0; col<block_size; col++){
					C[i+row][j+col] = block[row][col];
					printf("i+row=%d, j+col=%d, C(i+r,j+c)=%f \n" , i+row, j+col, C[i+row][j+col]);
					printf("row=%d, col=%d, bl(r,c)=%f \n" , row, col, block[row][col]);
					block[row][col] = 0;
					printf("%f\n", block[row][col]);
		   			printf ("n=%i, i=%i, j=%i block_size=%i\n", n, i, j, block_size);								
				}
			}
		}
	}
	for(int i=0; i<block_size; i++){		
		delete block[i];
	}
	delete [] block;

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
	int n = 512;//4096;
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


// 	int block_size = 4;
// 	reset(n, A, B, C);
// //start timer
// 	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
// 	block_matmul(n, A, B, C, block_size);
// //end timer 
// 	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}	
// 	display_results(n, start, stop, C);



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
