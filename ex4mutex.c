// Sincronização - Exercicio 1

#include <stdio.h>
#include <pthread.h>

#define MAX_THREADS 100

int contador_global = 0;
pthread_mutex_t mutex_;

void *thread (void *arg) {
	pthread_mutex_lock (&mutex_);
	int i;

	for(i=0; i<100; i++)
		contador_global++;

	pthread_mutex_unlock (&mutex_);
	return NULL;
}

int main(int argc, char **argv) {
	pthread_mutex_init (&mutex_, NULL);
	int i;
	pthread_t threads[MAX_THREADS];

	for(i=0; i < MAX_THREADS; i++)
		pthread_create(&threads[i], NULL, thread, NULL);

	for(i=0; i < MAX_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("Valor do contador global = %d\n", contador_global);
	pthread_mutex_destroy (&mutex_);
	return 0;
}

//while true; do ./ ...; sleep 1; done