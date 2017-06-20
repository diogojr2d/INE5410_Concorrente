#include <stdio.h>
#include <stdlib.h>

#define TAMANHO 3

void imprimeMatriz(int* m, int size) {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%d ", m[i*size+j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]) {
	unsigned int size = TAMANHO;
	if (argc > 1) {
		size = atoi(argv[1]);
		printf("Tamanho: %d\n", size);
	}
	int* matriz = (int*)malloc(sizeof(int)*size*size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matriz[i*size+j] = i*size+j;
		}
	}

	imprimeMatriz(matriz, size);

	return 0;
}