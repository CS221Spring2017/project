#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

int main(int argc, char*argv[])
{
	
	int size_mb = 256;
	long long size = size_mb * 1024 * 1024;
	long long *space = (long long*) malloc(size * sizeof(long long));
	memset(space, 1, size*sizeof(long long));
	unsigned long long begin, end, time;
	int nloops;
	//read bandwidth
	nloops = 10;
	begin = rdtsc();


	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size; i = i + 16)
		{

			long long n0 = space[i];
			long long n1 = space[i+1];
			long long n2 = space[i+2];
			long long n3 = space[i+3];
			long long n4 = space[i+4];
			long long n5 = space[i+5];
			long long n6 = space[i+6];
			long long n7 = space[i+7];
			long long n8 = space[i+8];
			long long n9 = space[i+9];
			long long n10 = space[i+10];
			long long n11 = space[i+11];
			long long n12 = space[i+12];
			long long n13 = space[i+13];
			long long n14 = space[i+14];
			long long n15 = space[i+15];

		}
	}

	end = rdtsc();
	time = end - begin;

	printf("read bandwidth(unrolled) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));
	
	long long delta = size/2;
	begin = rdtsc();
	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size/2; i = i + 16)
		{
			long long n0 = space[i];
			long long n1 = space[i+delta];
			long long n2 = space[i+1];
			long long n3 = space[i+1+delta];
			long long n4 = space[i+2];
			long long n5 = space[i+2+delta];
			long long n6 = space[i+3];
			long long n7 = space[i+3+delta];
			long long n8 = space[i+4];
			long long n9 = space[i+4+delta];
			long long n10 = space[i+5];
			long long n11 = space[i+5+delta];
			long long n12 = space[i+6];
			long long n13 = space[i+6+delta];
			long long n14 = space[i+7];
			long long n15 = space[i+7+delta];
		}
	}
	end = rdtsc();
	time = end - begin;
	printf("read bandwidth(unrolled+half) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));
	
	begin = rdtsc();
	long long val = 0;
	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size; i = i + 16)
		{
			val = space[i];
			val = space[i+1];
			val = space[i+2];
			val = space[i+3];
			val = space[i+4];
			val = space[i+5];
			val = space[i+6];
			val = space[i+7];
			val = space[i+8];
			val = space[i+9];
			val = space[i+10];
			val = space[i+11];
			val = space[i+12];
			val = space[i+13];
			val = space[i+14];
			val = space[i+15];
		}
	}
	long long a = val;
	end = rdtsc();
	time = end - begin;
	printf("read bandwidth(simple read) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));

	begin = rdtsc();
	val = 0;
	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size/2; i = i + 16)
		{
			val = space[i];
			val = space[i+delta];
			val = space[i+1];
			val  = space[i+1+delta];
			val = space[i+2];
			val = space[i+2+delta];
			val = space[i+3];
			val = space[i+3+delta];
			val = space[i+4];
			val = space[i+4+delta];
			val = space[i+5];
			val = space[i+5+delta];
			val = space[i+6];
			val = space[i+6+delta];
			val = space[i+7];
			val = space[i+7+delta];
		}
	}
	long long b = val;
	end = rdtsc();
	time = end - begin;
	printf("read bandwidth(simple read+half) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));
	

	begin = rdtsc();
	for(int j = 0; j < nloops; j++)
	{
		asm("cld\n"
		      "rep lodsq"
		       : : "S" (space), "c" (size) : "%eax");		
	}
	end = rdtsc();
	time = end - begin;
	printf("read bandwidth(asm) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));


	//write bandwidth
	begin = rdtsc();
	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size; i = i + 16)
		{
			space[i] = 1;
			space[i+1] = 2;
			space[i+2] = 3;
			space[i+3] = 4;
			space[i+4] = 5;
			space[i+5] = 6;
			space[i+6] = 7;
			space[i+7] = 8;
			space[i+8] = 9;
			space[i+9] = 10;
			space[i+10] = 11;
			space[i+11] = 12;
			space[i+12] = 13;
			space[i+13] = 14;
			space[i+14] = 15;
			space[i+15] = 16;
		}
	}
	end = rdtsc();
	time = end - begin;
	printf("write bandwidth(unrolled) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));

	begin = rdtsc();
	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size/2; i = i + 16)
		{
			space[i] = i;
			space[i+delta] = i+delta;
			space[i+1] = i+1;
			space[i+1+delta] = i+1+delta;
			space[i+2] = i+1+delta;
			space[i+2+delta] = i+2+delta;
			space[i+3] = i+4;
			space[i+3+delta] = i+3;
			space[i+4] = i+4;
			space[i+4+delta] = i+4;
			space[i+5] = i+5;
			space[i+5+delta] = i+5+delta;
			space[i+6] = i+6;
			space[i+6+delta] = i+6+delta;
			space[i+7] = i+7+delta;
			space[i+7+delta] = i+7+delta;
		}
	}
	end = rdtsc();
	time = end - begin;
	printf("write bandwidth(unrolled+half) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));

	begin = rdtsc();
	for (int j = 0; j < nloops; j++)
	{
		for(long long i = 0; i < size; i = i + 1)
		{
			space[i] = 100000000;
		}
	}
	end = rdtsc();
	time = end - begin;
	printf("write bandwidth(rolled) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));

	begin = rdtsc();
	for(int j = 0; j < nloops; j++)
	{
		memset(space, 0xff, size*sizeof(long long));
	}
	end = rdtsc();
	time = end - begin;
	printf("write bandwidth(memset) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));
	
	begin = rdtsc();
	for(int j = 0; j < nloops; j++)
	{
		asm("cld\n"
		      "rep stosq"
		       : : "D" (space), "c" (size) , "a" (0));		
	}
	end = rdtsc();
	time = end - begin;
	printf("write bandwidth(asm) = %f GB/s\n", sizeof(long long)*nloops*size_mb/1024/(time*0.34/1e9));

	
	free(space);
	return 0;
}