#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define	size 16*1024*1024
struct chunk {
  int start_idx;
  int end_idx;
};



void quickSort(int *array, int start, int end){
   // you quick sort function goes here  
	queue <int> q;
 	chunk temp;
 	temp.start_idx=start;
 	temp.end_idx=end;
 	q.emplace(temp);

 	while(q.empty() == false){
		temp= q.front();
	    start_idx = temp.start_idx;
	    end_idx = temp.end_idx;
 	}

   
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
	

	quickSort(m, 0, size);
		
			
	///////******************************////
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	
	for(i=0;i<16;i++) printf("%d ", m[i]);		
	printf("\nExecution time = %f sec\n",  exe_time);		
}	