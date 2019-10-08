#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define	num_of_points 40000000

struct Point{
	double x;  
	double y;
};

int main(void){
	int i; int j;
	int n_circle_pts;
	double pi;
	struct timespec start, stop; 
	double time;
	Point * data_point = (Point *) malloc (sizeof(Point)*num_of_points);
	for(i=0; i<num_of_points; i++){
		data_point[i].x=(double)rand()/(double)RAND_MAX; // double from 0..1
		data_point[i].y=(double)rand()/(double)RAND_MAX;
	}
	n_circle_pts=0;
	
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	
	
	////////**********Use OpenMP to parallize this loop***************//
	omp_set_num_threads(2); // Use 4 threads for all consecutive parallel regions
	#pragma omp parallel sections reduction(+:n_circle_pts)
	{
		#pragma omp section 
		for(i=0; i<num_of_points/2; i++){ 
			if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25){
				n_circle_pts++;
			}	
			if( i == (num_of_points/2)-1){
				printf("loop1, %f", n_circle_pts);
			}
		}	
		#pragma omp section
		for(j=num_of_points/2; j<num_of_points; j++){ 
			if((data_point[j].x-0.5)*(data_point[j].x-0.5)+(data_point[j].y-0.5)*(data_point[j].y-0.5)<=0.25){
				n_circle_pts++;
			}	
			if( j == num_of_points-1){
				printf("loop2\n", n_circle_pts);
			}
		}	
	}
	///////******************************////
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
	pi =4*(double)n_circle_pts/(double)num_of_points;
	printf("Estimated pi is %f, execution time = %f sec\n",  pi, time);		
}	