#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h> 
#include <pthread.h>

static inline unsigned long long rdtsc(void) {
	unsigned lo, hi;
	__asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
	return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

int main(int argc, char** argv)
{
	int nloops = atoi(argv[1]);
	unsigned long long time = 0;
	unsigned long long begin, end;
	//pid_t tid;
	uint64_t tid64;
	pthread_threadid_np(NULL, &tid64);

	for(int i = 0; i < nloops; i++)
	{
		uint64_t tid64;
		begin = rdtsc();
		pthread_threadid_np(NULL, &tid64);
		//tid = syscall(SYS_gettid);
		end = rdtsc();
		time += end - begin;
		printf("%llu\n",time);
		
	}
	printf("%llu cycles\n", time/nloops);
	return 0;
}