#include <time.h>
#include <stdio.h>
//#include <stdlib.h>
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


void block_matmul(int n, double**A, double**B, double** C, int block_size){
	double block[block_size][block_size];
	//clear block for each index C(i,j)
	for(int i=0; i<block_size; i++){				
		for(int j=0; j<block_size; j++){
			block[i][j] = 0;
		}
	}
	// double **block = new double*[block_size];
	// for(int i=0; i<block_size; i++){				
	// 	block[i] = new double[block_size];
	// 	for(int j=0; j<block_size; j++){
	// 		block[i][j] = 0;
	// 	}
	// }

	for(int i=0; i<n; i+=block_size){			//rows
		for(int j=0; j<n; j+=block_size){		//cols
			for(int k=0; k<n; k+=block_size){   
				//naive_matmul(block_size, , double **B, block_size)
				
				for(int ii=0; ii<block_size; ii++){
					for(int jj=0; jj<block_size; jj++){
						for(int kk=k; kk<block_size+k; kk++){
							//C[i+ii][j+jj] += A[i+ii][k+kk] * B[k+kk][j+jj];
							block[ii][jj] += A[i+ii][kk] * B[kk][j+jj];		
						}
					}
				}
			}

			//set C[i:i+blocksize, j:j+blocksize] = block and
			//clear block for each index C(i,j)
			for(int ii=0; ii<block_size; ii++){	
				for(int jj=0; jj<block_size; jj++){
					C[i+ii][j+jj] = block[ii][jj];
					block[ii][jj] = 0;
				}
			}
		}
	}
	// for(int i=0; i<block_size; i++){		
	// 	delete block[i];
	// }
	// delete [] block;

	return;
}

int main(){
	struct timespec start, stop;
	int n = 1024; //4096;
	//allocate memory
	double ** A = new double*[n];
	double ** B = new double*[n];
	double ** C = new double*[n];
	for (int i=0; i<n; i++){
		A[i] = new double[n];
		B[i] = new double[n];
		C[i] = new double[n];
	}

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
