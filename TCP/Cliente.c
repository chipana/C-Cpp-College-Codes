#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h> 


#define PORT 4444 
#define BUF_SIZE 2000 

void * receiveMessage(void * socket) {
	int sockfd, ret;
	char buffer[BUF_SIZE]; 
	sockfd = (int) socket;
	memset(buffer, 0, BUF_SIZE);
	int i = 1;
	while(i) {
		ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
		if (ret > 0) {
			fputs(buffer, stdout);
		} else {
			printf("Error receiving data!\n");
			i = 0;
		}  
	}
	free(socket);
}

int main() {  
	struct sockaddr_in addr, cl_addr;  
	int sockfd, ret;  
	char buffer[BUF_SIZE], message[BUF_SIZE], nickname[20],  serverAddr[16]; 
	pthread_t rThread;

	printf("Digite o IP do servidor: \n");  
	gets(serverAddr);
	printf("Digite o seu usuário: \n");  
	gets(nickname);
	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
	if (sockfd < 0) {  
		printf("Erro no socket\n");  
		exit(1);  
	}  
	printf("Socket............ OK!\n");   
	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = inet_addr(serverAddr);
	addr.sin_port = PORT;
	ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
	if (ret < 0) {  
		printf("Erro na conexao!\n");  
		exit(1);  
	}  
	printf("Conectado ao servidor %s!\n\n", serverAddr);  
	memset(buffer, 0, BUF_SIZE);
	printf("Digite sua mensagem e aperte enter para enviar\n");

	//creating a new thread for receiving messages from the server
	ret = pthread_create(&rThread, NULL, receiveMessage, (void *) sockfd);
	if (ret) {
		printf("ERRO %d : Falha na criacao da thread\n", ret);
		exit(1);
	}
	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		strcpy(message, nickname);
		strcat(message, ": ");
		strcat(message, buffer);
		ret = sendto(sockfd, message, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));  
		if (ret < 0)
			printf("Erro ao enviar dados!\n\n\t-%s", buffer);  
	}
	close(sockfd);
	pthread_exit(NULL);
}  
