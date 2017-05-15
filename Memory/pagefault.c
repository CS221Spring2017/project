#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

int main(int argc, char*argv[])
{
	long long offset = 16 * 1024 * 1024;
	long long filesize = 256 * 1024 * 1024;
	int nloop = 50;

	//create a file
	int file = open("random", O_CREAT|O_RDWR);
	if(file < 0)
	{
		printf("file creation failed\n");
		return -1;
	}
	//move the pointer n step forward(n = filesize-1)
	if(lseek(file, filesize -1, SEEK_SET) == -1)
	{
		close(file);
		return -1;
	}
	//write sth at current position
	if(write(file, "", 1) != 1)
	{
		close(file);
		return -1;
	}	
	//map the file into memory
	char * map = (char*) mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	char c;
	long long begin, end, time;

	begin = rdtsc();
	for(int i = 0; i < nloop; i++)
	{
		c = map[((i+1)*offset) %(filesize -1)];
	}
	end = rdtsc();
	time = end - begin;
	printf("Page Fault = %f ms\n", ((time-110) * 1.0 / nloop - 8 )*3.4e-7)  ;
	munmap(map, filesize);
	close(file);
	return 0;
}