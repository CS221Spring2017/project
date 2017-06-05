#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}
#define loops 1000

int main(int argc , char *argv[])
{

    int sockfd, newsockfd, portno,size;
    socklen_t clilen;
    
	if (argc < 3) {
         perror("usage : ./server port size\n");
         exit(1);
     }
    portno = atoi(argv[1]);
    size = atoi(argv[2]);

    //create socket
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR opening socket");

    //sockaddr_in structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              perror("ERROR on binding");

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    char *msg=(char*)malloc(size+1);
    memset (msg, 80, size+1);

    while(1)
    {
        newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
        
        if (newsockfd < 0) {
            perror("Error: fails to accept");
            return -1;
        }
        unsigned long long begin,end;
        unsigned long long diff=0;
        int n = 0;

        begin = rdtsc();
        
        for(int i=0;i < 10;i++) {
            n = recv(newsockfd, msg, size, MSG_WAITALL);
        }
        
        end = rdtsc();
        printf ("receive : %d\n", n);
        diff = end - begin;
        
        printf ("PEAK bandwidth is : %f MB/s \n", 10*(size/(1024*1.0*1024)) * (2.3e9 / diff) );
        
        close(newsockfd);
        return 0;
    }
    close(sockfd);

    puts("Finish calculating \n");
     
    return 0;
}
