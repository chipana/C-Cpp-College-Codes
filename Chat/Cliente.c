#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORTA 8080
int main(int argc, char *argv[]){
	int socket_srv, numbytes;
	char buf[2000], mensagem[2000], resposta[2000];
	struct hostent *he;
	struct sockaddr_in server;
	if (argc != 2) {
		fprintf(stderr,"Uso: cliente hostname\n");
		exit(1);
	}
	if ((he=gethostbyname(argv[1])) == NULL){
		herror("gethostbyname");
		exit(1);
	}
	if ((socket_srv = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORTA);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	//bzero(&(server.sin_zero), 8);
	if (connect(socket_srv,(struct sockaddr *)&server, sizeof(server)) < 0){
		perror("connect");
		exit(1);
	}
	puts("Conectado!!\n");
	while(1){
        printf("Digite a mensagem : ");
        scanf("%s" , mensagem);
        if(send(socket_srv, mensagem, strlen(mensagem), 0) < 0){
            puts("Send failed");
            return 1;
        }
        if( recv(socket_srv, resposta , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        puts("Resposta do servidor :");
        puts(resposta);
    }
	close(socket_srv);
	return 0;
}
