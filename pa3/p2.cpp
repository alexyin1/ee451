#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define		size	   16*1024*1024

void quickSort(int *array, int start, int end){
   // you quick sort function goes here  
   
}

int main(void){
	int i, j, tmp;
	struct timespec start, stop; 
	double exe_time;
	srand(time(NULL)); 
	int * m = (int *) malloc (sizeof(int)*size);
	for(i=0; i<size; i++){
		m[i]=size-i;
	}
	
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	////////**********Your code goes here***************//
	
		
			
	///////******************************////
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	
	for(i=0;i<16;i++) printf("%d ", m[i]);		
	printf("\nExecution time = %f sec\n",  exe_time);		
}	