#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORTA 4444
#define BUF_SIZE 2000
#define SIZE_END_CLIENTE 100

void * receberMensagem(void * socket) {
	int sockfd, recebeu;
	char buffer[BUF_SIZE]; 
	sockfd = (int) socket;
	memset(buffer, 0, BUF_SIZE);  
	while(1) {
		recebeu = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
		printf(recebeu);
		if (recebeu < 0) {  
			printf("Erro ao receber mensagem\n");    
		} else {
			fputs(buffer, stdout);
		}  
	}
}

void main() {
	struct sockaddr_in addr, cl_addr;
	int sockfd, tamanho, recebeu, novosockfd;
	char buffer[BUF_SIZE];
	pid_t childpid;
	char enderecoCliente[SIZE_END_CLIENTE];
	pthread_t rThread;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("erro ao criar socket!\n");
		exit(1);
	}
	printf("Socket criado\n");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = PORTA;

	recebeu = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (recebeu < 0) {
		printf("Erro de socket!\n");
		exit(1);
	}

	printf("Aguardando por uma conexao...\n");
	listen(sockfd, 5);


	tamanho = sizeof(cl_addr);
	novosockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &tamanho);
	if (novosockfd < 0) {
		printf("erro de conexao!\n");
		exit(1);
	} 

	inet_ntop(AF_INET, &(cl_addr.sin_addr), enderecoCliente, SIZE_END_CLIENTE);
	printf("Conexao aceita de: %s...\n", enderecoCliente); 

	memset(buffer, 0, BUF_SIZE);
	printf("Insira sua mensagem e tecle enter para enviar!\n");

	//criando a thread que recebe as mensagens do cliente
	recebeu = pthread_create(&rThread, NULL, receberMensagem, (void *) novosockfd);
	if (recebeu) {
		printf("erro ao criar a thread que recebe do cliente");
		exit(1);
	}

	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		recebeu = sendto(novosockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, tamanho);  
		if (recebeu < 0) {  
			printf("erro ao enviar a mensagem\n");  
			exit(1);
		}
	}   

	close(novosockfd);
	close(sockfd);

	pthread_exit(NULL);
	return;
}
