#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <pthread.h>

#define BILLION 1E9
#define NUM_THREADS 4
#define H 800 
#define W 800

#define input_file  "input.raw"
#define output_file "multithreaded.raw"

struct data_partition{
   int thread_id;
   int N;
   int num_clusters;
   unsigned char *data;
   unsigned int *cluster_dict;
   float *clusters;
   float *sums;
   float *counts;
};

struct data_partition data_partition_array[NUM_THREADS];
pthread_mutex_t mutexsum;

void *classify(void *partition_args){
	//shared vars
	struct data_partition * my_data;
	my_data = (struct data_partition *) partition_args;
	int thread_id = my_data->thread_id;
 	int N = my_data->N;
 	int num_clusters = my_data->num_clusters;
 	unsigned char *data = my_data->data;
 	unsigned int *cluster_dict = my_data->cluster_dict;
	float *clusters = my_data->clusters;
	float *sums = my_data->sums;
	float *counts = my_data->counts;
	//stack variables
	float partial_sums[] = {0,0,0,0};
	float partial_counts[] = {0,0,0,0};  
	float dpoint = 0; //  current datapoint value
	float best_dist = 255;
	float cur_dist;
	//printf("thread_id=%d, NUM_THREADS=%d, N=%d \n", thread_id, NUM_THREADS, N);
	for(unsigned int i = thread_id * N / NUM_THREADS; i < (thread_id+1) * N / NUM_THREADS; i++){
		best_dist = 255; //max value of 255
		for(unsigned int j=0; j<num_clusters; j++){
			dpoint = (float)data[i];
			cur_dist = (clusters[j] > dpoint) ? clusters[j] - dpoint : dpoint - clusters[j];
			if(cur_dist <= best_dist){
				//printf("here cur_dist=%f, best_dist=%f \n", cur_dist, best_dist);
				best_dist = cur_dist;			//  update min distance
				cluster_dict[i] = j;  //  update cluster category
			}
		}
		partial_sums[cluster_dict[i]] += (float) dpoint;
		partial_counts[cluster_dict[i]] += 1;
		// printf("cluster %d: partial_sums=%f , partial_counts=%f\n", cluster_dict[i], partial_sums[cluster_dict[i]], partial_counts[cluster_dict[i]]);
	}
	//lock
	pthread_mutex_lock (&mutexsum);
	for(unsigned int i=0; i<num_clusters; i++){
		sums[i] += partial_sums[i];
		counts[i] += partial_counts[i];
		//printf("cluster %d: partial_sums=%f , partial_counts=%f\n", cluster_dict[i], partial_sums[cluster_dict[i]], partial_counts[cluster_dict[i]]);
	}
	//unlock
    pthread_mutex_unlock (&mutexsum);
	pthread_exit(NULL);
}

void display_results(struct timespec start, struct timespec stop) {
	double time = (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec) / 1e9;
	printf("Execution time = %f sec,\n", time);
	return;
}


int main(int argc, char** argv){
	struct timespec start, stop;
    int i;
    FILE *fp;

  	unsigned char *data = (unsigned char*) malloc (sizeof(unsigned char)*H*W);

	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not open file\n");
		return 1;
	}
	fread(data, sizeof(unsigned char), W*H, fp);
	fclose(fp);

	if (clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime"); }
	unsigned int *cluster_dict = (unsigned int*) malloc (sizeof(unsigned int)*H*W);
	int num_iterations = 30;
	int num_clusters = 4;
	float clusters[] = {0, 85, 170, 255};
	float sums[] = {0,0,0,0};
	float counts[] = {0,0,0,0};  

    pthread_t  threads[NUM_THREADS];
    pthread_mutex_init(&mutexsum, NULL);
	unsigned int N = H*W;  //num_datapoints
	int rc;
	for (unsigned int iter=0; iter<num_iterations; iter++){
		//printf("ITER %u \n", iter);
		for (unsigned int i=0; i<NUM_THREADS; i++){
			data_partition_array[i].thread_id = i;
			data_partition_array[i].N = N;
			data_partition_array[i].num_clusters = num_clusters;
			data_partition_array[i].data = data;
			data_partition_array[i].cluster_dict = cluster_dict;
			data_partition_array[i].clusters = clusters;
			data_partition_array[i].sums = sums;
			data_partition_array[i].counts = counts;
			//classify(i, N, NUM_THREADS, num_clusters, data, cluster_dict, clusters, sums, counts);	
			rc = pthread_create(&threads[i], NULL, classify, (void *) &data_partition_array[i]);
			if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}
		}
		//synchronize here
		for(int j=0; j<NUM_THREADS; j++){
    	    pthread_join(threads[j], NULL);
	    }
		//lock
		for(unsigned int k=0; k<num_clusters; k++){
			// printf("sums=%f counts=%f \n", sums[k], counts[k]);
			clusters[k] = sums[k] / counts[k];
			if(iter == 29)	printf("cluster[%d] = %f\n", k, clusters[k]);
			sums[k] = 0;
			counts[k] = 0;
		}
		//unlock
	}

	// After n iterations, terminate the algorithm and output the matrix into the file named ‘output.raw’.
	for (unsigned int i=0; i<N; i++){
		data[i] = (unsigned char) clusters[cluster_dict[i]];	
		if(cluster_dict[i]!=0){
			//printf("data[%d] = cluster %d\n", i, cluster_dict[i]);
		}
	}

	if (clock_gettime(CLOCK_REALTIME, &stop) == -1) { perror("clock gettime"); }
	display_results(start, stop);
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not open file\n");
		return 1;
	}	
	fwrite(data, sizeof(unsigned char),N, fp);
    fclose(fp);
	pthread_exit(NULL);
}