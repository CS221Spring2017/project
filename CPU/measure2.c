#include <stdio.h>
#include <stdlib.h>
#include "rdtsc.h"

int main(int argc, char* argv[]){
	int nloops = atoi(argv[1]);

	unsigned long long begin;
	unsigned long long end;

	begin = rdtsc();
	
	for(int i = 0; i < nloops; i++){}

	end = rdtsc();
	unsigned long long elapse = end - begin;
	
	printf("%f\n",  elapse *1.0 / nloops);
}