#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	fork();

	printf("Novo processo criado!\n");

//	while(1){};
	wait(NULL);
	return 0;
}