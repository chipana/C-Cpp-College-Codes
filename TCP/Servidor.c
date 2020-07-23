#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 4444
#define BUF_SIZE 2000
#define CLADDR_LEN 100

void * receiveMessage(void * socket) {
	int sockfd, ret;
	char buffer[BUF_SIZE]; 
	sockfd = (int) socket;
	memset(buffer, 0, BUF_SIZE);
	int i = 1;
	while(i) {
		ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
		if (ret > 0) 
			fputs(buffer, stdout);
		else {
			printf("Error receiving data!\n");
			i = 0;
		}  
	}
	free(socket);
}
void main() {
	struct sockaddr_in addr, cl_addr;
	int sockfd, len, ret, newsockfd;
	char buffer[BUF_SIZE], nickname[20], message[BUF_SIZE];
	pid_t childpid;
	char clientAddr[CLADDR_LEN];
	pthread_t rThread;
	printf("Digite o seu usuário: \n");  
	gets(nickname);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("Erro no Socket");
		exit(1);
	}
	printf("Socket............ OK!\n");
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = PORT;
	ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0) {
		printf("Erro no binding!\n");
		exit(1);
	}
	printf("Binding.......... OK!\n");
	printf("Aguardando conexao...\n\n");
	listen(sockfd, 5);
	len = sizeof(cl_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
	if (newsockfd < 0) {
		printf("Erro na conexao!\n");
		exit(1);
	} 
	inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
	printf("Conexao recebida por %s...\n", clientAddr); 
	memset(buffer, 0, BUF_SIZE);
	printf("Digite sua mensagem e aperte enter para enviar\n");
	ret = pthread_create(&rThread, NULL, receiveMessage, (void *) newsockfd);
	if (ret) {
		printf("ERRO %d : Falha na criacao da thread\n", ret);
		exit(1);
	}
	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		strcpy(message, nickname);
		strcat(message, ": ");
		strcat(message, buffer);
		ret = sendto(newsockfd, message, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);  
		if (ret < 0) {  
			printf("Erro ao enviar dados!\n");  
			exit(1);
		}
	}   
	close(newsockfd);
	close(sockfd);
	pthread_exit(NULL);
}
