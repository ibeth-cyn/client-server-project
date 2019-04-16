#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include "rdwrn.h"

#define SIZE 10

void get_student_info(int socket)
{
    char value[100];
    size_t k;
    
    readn(socket, (unsigned char *) &k, sizeof(size_t));	
    readn(socket, (unsigned char *) value, k);
    printf("IP Address and student Info: %s\n", value);
    printf("Received: %zu bytes\n\n", k);
}

void get_random_number(int socket)
{
    char str[50];
    size_t l;

    readn(socket, (unsigned char *) &l, sizeof(size_t));
    readn(socket, (unsigned char *) str, l);
    printf("%s\n", str);
    printf("Received: %zu bytes\n\n", l);
}

void get_utsname_info(int socket)
{
    char utsinfo[256];
    size_t m;

    readn(socket, (unsigned char *) &m, sizeof(size_t));
    readn(socket, (unsigned char *) utsinfo, m);
    printf("%s", utsinfo);
    printf("Received: %zu bytes\n\n", m);
}

void send_choice(int socket, char *in[SIZE])
{
     writen(socket, (unsigned char *) in, sizeof(size_t));
}
void display_menu_options()
{
     printf("----------------------------------------------------------\n");
     printf("Welcome\n");
     printf("Enter a number to display the corresponding information\n");
     printf("0 - Display IP Address and Student Info\n");
     printf("1 - Display 5 random numbers\n");
     printf("2 - Display System information\n");
     printf("3 - Display File Names in the upload directory\n");
     printf("4 - Exit the application\n");
}

int main(void)
{
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Error - could not create socket");
	exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(50031);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)  {
	perror("Error - connect failed");
	exit(1);
    } else
       printf("Connected to server...\n");

    display_menu_options();
    char menu_options[SIZE];
    char choice;
    do{
       printf("Enter your choice here >>> \n");
       fgets(menu_options, SIZE, stdin);
       menu_options[strcspn(menu_options, "\n")] = 0;
       choice = menu_options[0];
       if (strlen(menu_options) > 1)
          choice = 'x';

       send_choice(sockfd, &menu_options);
       
       switch(choice){
        case '0':
            get_student_info(sockfd);
            break;
        case '1':
            get_random_number(sockfd);
            break;
        case '2':
            get_utsname_info(sockfd);
            break;
        case '3':
            printf("You have not written this method\n");
            break;
        case '4':
            printf("Exit gracefully\n");
            break;
        default:
            printf("You entered a wrong choice\n");
            break;
        } 
     }while(choice != '4');
    

    close(sockfd);

    exit(EXIT_SUCCESS);
}
