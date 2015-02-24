/*	
	DCT Reference: http://www.mathworks.com/help/signal/ref/dct.html
	Single Thread Reference: https://github.com/matthewsears/NUCAR/blob/master/DCT/cpu/dct.cpp
	Author: Matthew Sears 
			sears.ma@husky.neu.edu
 */
#include <iostream>
#include <new>
#include <math.h>
#include <pthread.h>

#define numThreads 10
#define M_PI_2		1.57079632679489661923

using namespace std;

typedef struct 
{
	int ID;
	float *input  = new float[numThreads]; 
	
} dctInfo;

void *dct_1d(void *info)
{
	dctInfo *data;
	data = (dctInfo *) info;
	float *output_ptr;	
	float alpha;
	float alpha0 = 1.f / sqrt((float)numThreads);
	float alpha1 = sqrt(2.f / numThreads);
	float sum = 0.f;
	
	alpha = (data->ID == 1) ? alpha0 : alpha1;
	
	for(int n=0; n<numThreads; n++) 
		sum += alpha * (data->input[n]) * cos(M_PI_2 * (2 * n + 1) * (data->ID) / numThreads);
	
	output_ptr = &sum;
	
	pthread_exit(output_ptr);
}

int main(int argc, char *argv[])
{
	int ret;
	pthread_t dct_threads[numThreads];
	dctInfo thread[numThreads];
	void *output_ret;
	float *output = new float[numThreads];	
	
	
	/* Create threads with IDs*/
	for(int i=0; i<numThreads; i++) 
	{
		cout << "Creating thread #" << i + 1<< endl;
		ret = pthread_create(&dct_threads[i], NULL, dct_1d, &thread[i]); // Pass struct as arg
		if(ret) { cout << "Error creating thread" << endl; }             // Err check
		thread[i].ID = i + 1; 						 // IDs from 1-10, not 0-9
	}
	
		/* Test data */
	for(int i=0; i<numThreads; i++) 
	{
		for(int j=0; j<numThreads; j++)
			thread[i].input[j] = (float) (j+1); 
	}
	
	/* Join threads and receive return */
	for(int i=0; i<numThreads; i++)
	{
		cout << "Joining thread #" << i + 1<< endl;
		ret = pthread_join(dct_threads[i], &output_ret);				// Pointer to output sum will return
				if(ret) { cout << "Error joining thread" << endl; }		// Err check
		output[i] = *(float *)output_ret;	
	}	
	
	/* Print data/results */
	cout << "Input:" << endl;	
	for(int i=0; i<numThreads; i++) 
	{
		for(int j=0; j<numThreads; j++)
			cout << thread[i].input[j] << " ";
		cout << endl; 
	}
	
	cout << endl << "Output:" << endl;
	for(int i=0; i<numThreads; i++) 
	{
		cout << output[i] << " "; 
	}
	cout << endl;
	

	/* Clean up */
	for(int i=0; i<numThreads; i++) 
		delete [] thread[i].input;
	delete [] output;
	
	return 0;
}
