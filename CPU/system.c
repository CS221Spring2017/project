#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#define _GNU_SOURCE         
#include <sys/syscall.h>   

static inline unsigned long long rdtsc(void) {
	unsigned lo, hi;
	__asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
	return (((unsigned long long int)hi << 32) | lo);
}

int main(int argc, char** argv)
{
	int nloops = atoi(argv[1]);
	unsigned long long time = 0;
	unsigned long long begin, end;

	for(int i = 0; i < nloops; i++)
	{
		pid_t tid;
		begin = rdtsc();
		tid = syscall(SYS_gettid);
		end = rdtsc();
		time = end - begin;
		printf("%llu cycles\n", time);	
			return 0;		
	}
	return 0;
}