#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

const off_t BLOCK_SIZE = 4 * 1024;

int main(int argc, const char* argv[])
{
	if(argc < 2)
	{
		printf("usage: ./cache 4 M cache4M\n");
		exit(0);
	}
	off_t FILE_SIZE = atoll(argv[1]);
	const char type = *argv[2];
	if(type == 'G')	
		FILE_SIZE = FILE_SIZE * 1024 * 1024 * 1024;
	if(type == 'M')
		FILE_SIZE = FILE_SIZE * 1024 * 1024;

	void *buffer = malloc(BLOCK_SIZE);
	unsigned long long total_bytes = 0;
	unsigned long long begin;
	unsigned long long end;
	unsigned long long total;

	//read file into cache
	int file = open(argv[3], O_RDONLY);	
	if(lseek(file, FILE_SIZE -1, SEEK_SET) == -1)
	{
		close(file);
		return -1;
	}
	while(1){
		lseek(file, -2 * BLOCK_SIZE, SEEK_CUR);
		ssize_t bytes = read(file, buffer, BLOCK_SIZE);
		if(bytes <= 0 || total_bytes >= FILE_SIZE)
			break;
		total_bytes += bytes;
	}
	close(file);
    
	//Read the file again
	file = open(argv[3], O_RDONLY);
	if(lseek(file, FILE_SIZE -1, SEEK_SET) == -1)
	{
		close(file);
		return -1;
	}
	while(1)
	{
		lseek(file, -2*BLOCK_SIZE, SEEK_CUR);
		begin = rdtsc();
		ssize_t bytes = read(file, buffer, BLOCK_SIZE);
		end = rdtsc();
		total += end - begin;
		if(bytes <= 0 || total_bytes >= FILE_SIZE)
			break;
		total_bytes += bytes;
	}
	close(file);
	free(buffer);

	double answer = total*1.0/(FILE_SIZE*1.0/BLOCK_SIZE);
	printf("file size = %d MB, cycle = %lf \n", (int)FILE_SIZE/(1024*1024), answer);
}