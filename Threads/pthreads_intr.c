
#include <stdio.h>
#include <pthread.h>
#define NUMTHRDS 2
pthread_t t [ NUMTHRDS];
int coin_flip;
static void *thread2(void *_)
{
	printf("Thread 2: flipped coin %d\n", coin_flip);
}
static void *thread1(void *_)
{

	coin_flip = 23;
	printf("Thread 1: flipped coin %d\n", coin_flip);
}
int main()
{	
	 //Nao sabemos qual thread será executada primeiro
	
	pthread_create(&t[1], NULL, thread2, NULL);
	pthread_create(&t[0], NULL, thread1, NULL);

	//sem esta instrução o código ficaria bloquedo até o término das threads
	pthread_exit(NULL);

	return 1;
}
