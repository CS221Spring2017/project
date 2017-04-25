#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

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
		pid_t pid = fork();
		if(pid == 0)
		{
			begin = rdtsc();
			getpid();
			end = rdtsc(
				);
			time += end - begin;
			printf("%llu\n\n", time);	
			return 0;		
		}
		if(pid == -1)
		{
			printf("fork faliled\n");
			return 0;
		}
	}	
	return 0;
}