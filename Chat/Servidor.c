#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define MINHAPORTA 8080    /* Porta que os usuarios irão se conectar*/
int main()
{
	int socket_server, socket_client, tam_w, tam_r;
	struct sockaddr_in end_server, end_client;
    char msg_client[2000];
	socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1){
		perror("socket");
		exit(1);
	}
	end_server.sin_family = AF_INET;
	end_server.sin_addr.s_addr = INADDR_ANY;
	end_server.sin_port = htons(MINHAPORTA);
	//bzero(&(end_server.sin_zero), 8);
    //BIND
	if (bind(socket_server, (struct sockaddr *)&end_server, sizeof(end_server)) < 0){
		perror("BBind");
		exit(1);
	}
	puts("Bind OK!");
    //LISTEN
	if (listen(socket_server, 1) < 0){
		perror("listen");
		exit(1);
	}
	puts("Listen OK!");
	puts("Esperando conexoes... ");
	while(1){
		tam_w = sizeof(struct sockaddr_in);
		socket_client = accept(socket_server, (struct sockaddr *)&end_client,(socklen_t*)&tam_w);
		if (socket_client < 0){
			perror("accept");
			return 1;
		}
		puts("Conexão aceita!");
		while((tam_w = recv(socket_client,msg_client, 2000, 0)) > 0){
			puts("Mensagem do cliente: ");
			puts(msg_client);
			msg_client[tam_w] = '\0';
			fprintf("Client message: %s", msg_client);			
			write(socket_client, msg_client, strlen(msg_client));
			if(tam_w == 0)
				puts("Cliente desconectado");
			else if(tam_w == -1)
				puts("Falha no recebimento!");
		}
	}
}
