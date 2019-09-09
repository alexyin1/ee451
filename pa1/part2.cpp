#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define H 800 
#define W 800

#define input_file  "input.raw"
#define output_file "output1.raw"

int main(int argc, char** argv){
    int i;
    FILE *fp;

  	unsigned char *data = (unsigned char*) malloc (sizeof(unsigned char)*H*W);

	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not opern file\n");
		return 1;
	}
	fread(data, sizeof(unsigned char), W*H, fp);
	fclose(fp);
    
	// measure the start time here
	
	//  Your code goes here
	unsigned int *cluster_dict = (unsigned int*) malloc (sizeof(unsigned int)*H*W);
	int num_iterations = 30;
	int num_clusters = 4;
	//1. Initialize a mean value for each cluster.
	float clusters[] = {0, 85, 170, 255};
	float sums[] = {0,0,0,0};
	float counts[] = {0,0,0,0};
  
/*


4. Check convergence; if the algorithm converges, replace the value of each data with the
mean value of the cluster which the data belongs to, then terminate the algorithm;
otherwise, go to Step 2.
*/
	float dist = 255;
	float cur_dist;
	float dpoint = 0; //  current datapoint
	for (unsigned int iter=0; iter<num_iterations; iter++){
		//  2. For each data element, compute its ‘distance’ to the mean value of each cluster. Assign
		//  it to the ‘closest’ cluster.
		printf("ITER %u \n", iter);
		for (unsigned int i=0; i<H*W; i++){
			dist = 255;
			for(unsigned int j=0; j<num_clusters; j++){
				dpoint = (float)data[i];
				cur_dist = (clusters[j] > dpoint) ? clusters[j] - dpoint : dpoint - clusters[j];
				if(cur_dist <= dist){
					dist = cur_dist;			//  update min distance
					cluster_dict[i] = j;  //  update cluster category
				}
			}
			//  3. After each data element is assigned to a cluster, recompute the mean value of each cluster.
			sums[cluster_dict[i]] += (float) dpoint;
			counts[cluster_dict[i]] += 1;
		}
		for(unsigned int k=0; k<num_clusters; k++){
			clusters[k] = sums[k] / counts[k];
			sums[k] = 0;
			counts[k] = 0;
		}
	}
	

	// After n iterations, terminate the algorithm and output the matrix into the file named ‘output.raw’.
	for (unsigned int i=0; i<H*W; i++){
		data[i] = (unsigned char) clusters[cluster_dict[i]];	
	}
	//printf("H*W%i", H*W);
	//
	
	// measure the end time here
	
	// print out the execution time here
	
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not opern file\n");
		return 1;
	}	
	fwrite(data, sizeof(unsigned char),W*H, fp);
    fclose(fp);
    
    return 0;
}