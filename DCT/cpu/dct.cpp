/*	
	DCT Reference: http://www.mathworks.com/help/signal/ref/dct.html
	Author: Leiming Yu 
            ylm@ece.neu.edu
 */
#include <iostream>
#include <new>
#include <math.h>

using namespace std;

void dct_1d(float *in, float *out, int N)
{
	float alpha;
	float alpha0 = 1.f / sqrt((float)N);
	float alpha1 = sqrt(2.f / N);

	for(int k=0; k<N; k++)
	{
		alpha = (k ==0)? alpha0 : alpha1;

		float sum = 0.f;
		for(int n=0; n<N; n++)	
		{	
			sum += alpha * in[n] * cos( M_PI_2 * (2 * n + 1) * k / N); 
		}

		out[k] = sum;
	}
}

int main(int argc, char *argv[])
{
	int N = 10;
	float *input  = new float[N]; 
	float *output = new float[N]; 

	for(int i=0; i<N; i++)
		input[i] = (float) (i+1);

	cout << "input" << endl;
	for(int i=0; i<N; i++)
	{
		cout << input[i] << " ";
	}
	cout << endl;

	dct_1d(input, output, N);
	
	for(int i=0; i<N; i++)
	{
		cout << output[i] << " ";
	}
	cout << endl;

	delete [] input;
	delete [] output;

	return 0;
}
