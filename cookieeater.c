#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int cookieeater(){
    
    char arg_ip[];
	char arg_ip_raw[INET6_ADDRSTRLEN];
	char arg_message_raw[10];
    
    printf("ip: ");
	fgets(arg_ip_raw, INET6_ADDRSTRLEN, stdin);

	arg_ip = arg_ip_raw;

    printf("message: ");
	fgets(arg_message_raw, 10, stdin);

    sendMessage(arg_ip, arg_message_raw);

    char requestIp[INET6_ADDRSTRLEN];
    waitForMessage(&requestIp);

    return 0;
}

int waitForMessage(char requestIp[]){
    printf("waiting for response...\n");
	int numbytes;

    int sockfd;
	struct addrinfo addrinfoPrefs, *servinfo, *p;

	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	char recievedInfo[100];

    memset(&addrinfoPrefs, 0, sizeof addrinfoPrefs);
    addrinfoPrefs.ai_socktype = SOCK_DGRAM;
    addrinfoPrefs.ai_flags = AI_PASSIVE;

    int getaddrinfoResult = getaddrinfo(NULL, "4950", &addrinfoPrefs, &servinfo);
    if(getaddrinfoResult != 0){
        printf("Could not get address information!\n");
        return 1;
    }
    
    for(p = servinfo; p != NULL; p = p->ai_next){
        sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
        if((sockfd) > -1){
            bind(sockfd, p->ai_addr, p->ai_addrlen);
            break;
        }
    }
    if (p == NULL) {
        printf("Could not create a socket!\n");
        return 1;
    }

    addr_len = sizeof their_addr;

    int recvfromResult = recvfrom(sockfd, recievedInfo, 99 , 0, (struct sockaddr *)&their_addr, &addr_len);
    if (recvfromResult == -1) {
		printf("Could not get socket information!\n");
        return 1;
	}

    printf("they responded!\n");
    printf("they said %s\n", recievedInfo);

    freeaddrinfo(servinfo);
    close(sockfd);
    return 0;
}


int sendMessage(char ip[], char message[]){
    int sockfd;
    struct addrinfo addrinfoPrefs, *servinfo, *p;

    memset(&addrinfoPrefs, 0, sizeof addrinfoPrefs);
    addrinfoPrefs.ai_family = AF_INET;
    addrinfoPrefs.ai_socktype = SOCK_DGRAM;

    int getaddrinfoResult = getaddrinfo(ip, "4950", &addrinfoPrefs, &servinfo);
    if(getaddrinfoResult != 0){
        printf("Could not get address information!\n");
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next){
        sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
        if(sockfd != -1){
            break;
        }
    }
    if (p == NULL) {
        printf("Could not create a socket!\n");
        return 1;
    }

    int sendtoResult = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);
    if (sendtoResult == -1) {
		printf("Could not send message!\n");
        return 1;
	}

    freeaddrinfo(servinfo);
    close(sockfd);
    printf("asking for a cookie!\n");
}