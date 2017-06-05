#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>


static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}
#define loops 1

int main(int argc , char *argv[])
{      
    int sockfd, port, n,size_MB;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port size\n", argv[0]);
       exit(0);
    }

    //get port size and create socket
    port = atoi(argv[2]);
    size_MB = atoi(argv[3])*1024*102;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(0);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //connection between server and client
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        perror("ERROR connecting");

    //calcaulate round time
    char *msg = (char*)malloc(size_MB);

    unsigned long long begin,end;
    unsigned long long diff = 0;
    //int counter=size;

    begin = rdtsc();

    for(int i=0;i < loops;i++) {
        if(send(sockfd, msg, size_MB, 0) < 0) {
            perror("send failed");
            return -1;
        }
    }
    
    end = rdtsc();
    //printf ("send : %d\n", counter);
    diff = end - begin;

    close(sockfd);

    printf ("PEAK bandwidth is : %f MB/s \n", (size_MB*loops) / (0.34*diff/1e9) );
    
    return 0;

}
