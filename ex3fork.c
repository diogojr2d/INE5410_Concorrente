#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	pid_t pid = getpid();
	
	for (int i = 0; i < 2; i++) {
		if (pid > 0) {
			pid = fork();
		} else {
			fork();
		}
	}
/*	printf("Criou pid: %d \n ", pid);
	if (pid > 0) {
		pid = fork();
//		printf("Criou pid: %d \n ", pid);
	}
	if (pid == 0) {
		pid = fork();
//		printf("Criou pid: %d \n ", pid);
		if (pid > 0) {
			pid = fork();
//			printf("Criou pid: %d \n ", pid);
		}
	}
*/
	while(1);
	while(wait(NULL) != -1);
	return 0;
}