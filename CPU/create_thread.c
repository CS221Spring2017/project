#include "stdlib.h"
#include "stdio.h"
#include <pthread.h>
#include <unistd.h>

static inline unsigned long long rdtsc(void) {
	unsigned lo, hi;
	__asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
	return (((unsigned long long int)hi << 32) | lo);
}

void *foo(void *argv){
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	int nloops = atoi(argv[1]);
	unsigned long long time = 0;
	unsigned long long begin, end;

	for(int i = 0; i< nloops; i++)
	{
		pthread_t thread;
		long temp;
		begin = rdtsc();
		pthread_create(&thread, NULL, foo, (void *) temp);
		end = rdtsc();
		time += (end - begin);
		pthread_join(thread, NULL);
	}
	printf("%llu cycles\n", time / nloops);
	return 0;
}