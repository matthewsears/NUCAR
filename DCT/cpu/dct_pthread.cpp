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
#include <stdlib.h>

#define numThreads 3 

#define DEBUG 0

using namespace std;

typedef struct 
{
	float *input;
	float *output;
	int tid;
	int size;
} dctInfo;

void *dct_1d(void *dct_info)
{
	dctInfo *data;
	data = (dctInfo *) dct_info;

	float *input  = data->input; 
	float *output = data->output; 
	int N         = data->size;
	int tid       = data->tid;

	float alpha;
	float alpha0 = 1.f / sqrt((float)N);
	float alpha1 = sqrt(2.f / N);


	/* Divide N data points to numThreads */
	int stepsize = ceil( (float)N / numThreads);
	int start = tid * stepsize; 
	int end = (start + stepsize <= N) ? start + stepsize - 1: N - 1;

#if DEBUG
	//cout << stepsize << endl;
	//cout << start << endl;
#endif	

	for(int k=start; k<=end; k++)
	{
		alpha = (k==0)? alpha0 : alpha1;	
		float sum = 0.f;
		for(int n=0; n<N; n++) 
			sum += alpha * input[n] * cos(M_PI_2 * (2 * n + 1) * k / N);
		output[k] = sum;
	}
}

int main(int argc, char *argv[])
{
	/* Initialize input*/
	int N = 10;
	float *input = new float[N];
	for(int i=0; i<N; i++)
	{
		input[i] = (float) (i+1);
	}

	float *output = new float[N];

	pthread_t dct_threads[numThreads];
	pthread_attr_t attr;
	dctInfo dct_info[numThreads];
	int  ret;
	void *status;

	/* Initialze and configure the attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Create threads with IDs*/
	for(int t=0; t<numThreads; t++) 
	{
		dct_info[t].size   = N;
		dct_info[t].tid    = t;
		dct_info[t].input  = input;
		dct_info[t].output = output;

		ret = pthread_create(&dct_threads[t], NULL, dct_1d, (void *)&dct_info[t]);
		if(ret) 
		{ 
			cout << "Error creating thread %d\n" << t << endl; 
			exit(-1);
		}
	}
	
	/* Free attribute */
	pthread_attr_destroy(&attr);


	/* Join threads and receive return */
	for(int i=0; i<numThreads; i++)
	{
		ret = pthread_join(dct_threads[i], &status);
		if(ret) 
		{ 
			cout << "Error joining thread" << endl; 
			exit(-1);
		}
		// report status
		//cout << "Thread " << i << " completed join with a status " << (long) status << endl;
	}	
	
	cout << endl << "Output:" << endl;
	for(int i=0; i<N; i++) 
	{
		cout << output[i] << " "; 
	}
	cout << endl;

	delete [] input;
	delete [] output;

	pthread_exit(NULL);
	
	return 0;
}
