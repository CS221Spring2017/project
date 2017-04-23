#include <stdio.h>
#include <stdlib.h>
#include "rdtsc.h"

int main(int argc, char* argv[]){
	int nloops = atoi(argv[1]);

	unsigned long long begin;
	unsigned long long end;
	unsigned long long sum = 0;

	for(int i = 0; i < nloops; i++)
	{
		begin = rdtsc();
		end = rdtsc();
		sum += (end - begin);
	}
	printf("%f\n", sum*1.0 / nloops);
}