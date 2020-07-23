#include <stdio.h>
#include <pthread.h>
#define NUMTHRDS 2
pthread_t t [NUMTHRDS];
int coin_flip;
pthread_mutex_t flip_done;
static void *thread2(void *_){
	pthread_mutex_lock(&flip_done);
	coin_flip = coin_flip +1;
	printf("Thread 2: flipped coin %d\n", coin_flip);
}
static void * thread1(void * _)
{
	coin_flip = 23;
	printf("Thread 1: flipped coin %d\n", coin_flip);
	pthread_mutex_unlock(&flip_done);
}

int main()
{
	pthread_mutex_init(&flip_done, NULL);
	pthread_mutex_lock(&flip_done);
	pthread_create(&t[0], NULL, thread2, NULL);
	pthread_create(&t[1], NULL, thread1, NULL);

	pthread_mutex_destroy(&flip_done);

	pthread_exit(NULL);

	return 1;
}
