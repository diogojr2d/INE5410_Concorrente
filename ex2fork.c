#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	pid_t pid = getpid();
	for (int i = 0; i < 4; i++) {
		if (pid > 0) {
			pid = fork();
			if (pid == 0) {
				printf("Processo filho %d \n", getpid());
				break;
			} else if (pid > 0) {
				printf("Processo pai %d criou %d \n", getpid(), pid);
			} else {
				printf("Processo não pode ser criado.\n");
				break;
			}
		}
	}

//	while(1);
	while(wait(NULL) != -1);
	return 0;
}