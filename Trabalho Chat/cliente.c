#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
#include"pthread.h"
  
#define PORTA 4444 
#define BUF_SIZE 2000 
  
void * receberMensagem(void * socket) {
 int sockfd, recebeu;
 char buffer[BUF_SIZE]; 
 sockfd = (int) socket;
 memset(buffer, 0, BUF_SIZE);  
 for (;;) {
  recebeu = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
  if (recebeu < 0) {  
   printf("erro ao receber mensagem!\n");    
  } else {
   printf("servidor: ");
   fputs(buffer, stdout);
   //printf("\n");
  }  
 }
}

int main(int argc, char**argv) {  
 struct sockaddr_in addr, cl_addr;  
 int sockfd, recebeu;  
 char buffer[BUF_SIZE]; 
 char * enderecoServidor;
 pthread_t rThread;

 if (argc < 2) {
  printf("para executar o cliente, digite: ./cliente <endereco ip do servidor>\n");
  exit(1);  
 }

 enderecoServidor = argv[1]; 
 
 sockfd = socket(AF_INET, SOCK_STREAM, 0);  
 if (sockfd < 0) {  
  printf("erro ao criar socket!\n");  
  exit(1);  
 }  
 printf("Socket criado...\n");   

 memset(&addr, 0, sizeof(addr));  
 addr.sin_family = AF_INET;  
 addr.sin_addr.s_addr = inet_addr(enderecoServidor);
 addr.sin_port = PORTA;     

 recebeu = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
 if (recebeu < 0) {  
  printf("erro ao conectar com o servidor!\n");  
  exit(1);  
 }  
 printf("Conectado ao servidor...\n");  

 memset(buffer, 0, BUF_SIZE);
 printf("Insira sua mensagem e tecle enter para enviar!\n");

 //criando a thread que recebe as mensagens do Servidor
 recebeu = pthread_create(&rThread, NULL, receberMensagem, (void *) sockfd);
 if (recebeu) {
  printf("erro ao criar a thread que recebe do Servidor");
  exit(1);
 }

 while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
  recebeu = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));  
  if (recebeu < 0) {  
   printf("erro ao enviar mensagem!\n\t-%s", buffer);  
  }
 }

 close(sockfd);
 pthread_exit(NULL);
 
 return 0;    
}  
