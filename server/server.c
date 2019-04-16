#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/utsname.h>
#include <signal.h>
#include<time.h>
#include <sys/time.h>
#include <dirent.h>
#include "rdwrn.h"

#define SIZE 10

 struct timeval tv1, tv2;
void *client_handler(void *);

void send_student_info(int);
void send_random_number(int);
void send_utsname_info(int);

char get_user_choice(int);
static void handler(int, siginfo_t *, void *);

int main(void)
{
    struct sigaction act;

    memset(&act, '\0', sizeof(act));

    act.sa_sigaction = &handler;

    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &act, NULL) == -1) {
	perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (gettimeofday(&tv1, NULL) == -1) {
	perror("gettimeofday error");
	exit(EXIT_FAILURE);
    }

    int listenfd = 0, connfd = 0;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    socklen_t socksize = sizeof(struct sockaddr_in);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(50031);

    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    if (listen(listenfd, 10) == -1) {
	perror("Failed to listen");
	exit(EXIT_FAILURE);
    }

    puts("Waiting for incoming connections...");
    while (1) {
	printf("Waiting for a client to connect...\n");
	connfd =
	    accept(listenfd, (struct sockaddr *) &client_addr, &socksize);
	printf("Connection accepted...\n");

	pthread_t sniffer_thread;
	if (pthread_create
	    (&sniffer_thread, NULL, client_handler,
	     (void *) &connfd) < 0) {
	    perror("could not create thread");
	    exit(EXIT_FAILURE);
	}
	pthread_join( sniffer_thread , NULL);
	printf("Handler assigned\n");
    }

    exit(EXIT_SUCCESS);
}

void *client_handler(void *socket_desc)
{
    int connfd = *(int *) socket_desc;
    
    char options;

    do{
        options = get_user_choice(connfd);

        switch(options){
          case '0':
            send_student_info(connfd);
            break;
          case '1':
            send_random_number(connfd);
            break;
          case '2':
            send_utsname_info(connfd);
            break;
          case '3':
            printf("You have not written this method");
            break;
          case '4':
            printf("Exit gracefully");
            break;
          case 's':
            printf("Quit gracefully");
            break;
          default:
            printf("You entered a wrong choice");
            break;
       }
    }while(options != '4' || options != 's');

    shutdown(connfd, SHUT_RDWR);
    close(connfd);

    printf("Thread %lu exiting\n", (unsigned long) pthread_self());

    shutdown(connfd, SHUT_RDWR);
    close(connfd);

    return 0;
}

void send_student_info(int socket)
{
    struct sockaddr_in serv_addr;
    char *ip;
    char name[] = ", Elizabeth Michael Akpan";
    char studentID[] = ", S1719014";
    ip = inet_ntoa(serv_addr.sin_addr);
    char value[100];
    strcpy(value, ip);
    strcat(value, name);
    strcat(value, studentID);
    size_t n = strlen(value) + 1;
    writen(socket, (unsigned char *) &n, sizeof(size_t));	
    writen(socket, (unsigned char *) value, n);
}

void send_random_number(int socket)
{
    int num;
    char str[50];
    char num_str[10];
    strcpy(str, "The 5 Random values are: \n");
    size_t n;
    int i;
    for(i = 0; i < 5; i++){
        num = rand() % 1001;
        sprintf(num_str, "%d ", num);
        strcat(str, num_str);
    }
    n = strlen(str) + 1;
    writen(socket, (unsigned char *) &n, sizeof(size_t));
    writen(socket, (unsigned char *) str, n);
}

void send_utsname_info(int socket)
{
    struct utsname uts;

    if (uname(&uts) == -1) {
	perror("uname error");
    }

    char sysinfo [256];
    strcat(sysinfo, "Nodename : ");
    strcat(sysinfo, uts.nodename);
    strcat(sysinfo,"\n");
    strcat(sysinfo, "System Name : ");
    strcat(sysinfo, uts.sysname);
    strcat(sysinfo,"\n");
    strcat(sysinfo, "Release : ");
    strcat(sysinfo, uts.release);
    strcat(sysinfo,"\n");
    strcat(sysinfo, "Version : ");
    strcat(sysinfo, uts.version);
    strcat(sysinfo,"\n");
    strcat(sysinfo, "Machine : ");
    strcat(sysinfo, uts.machine);
    size_t n = strlen(sysinfo) + 1;

    writen(socket, (unsigned char *) &n, sizeof(size_t));
    writen(socket, (unsigned char *) sysinfo, n);

}

char get_user_choice(int socket)
{
    char input[SIZE];
    readn(socket, (unsigned char *) input,sizeof(size_t));
    printf("%s\n", input);
    return input[0];
}

static void handler(int sig, siginfo_t *siginfo, void *context)
{
    if (gettimeofday(&tv2, NULL) == -1) {
	perror("gettimeofday error");
	exit(EXIT_FAILURE);
    }

    printf("Total execution time = %f seconds\n",
	   (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
	   (double) (tv2.tv_sec - tv1.tv_sec));
    
    exit(130);
}
