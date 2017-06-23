#include <stdio.h>
#include <mpi.h>

/*
Faça um programa usando MPI que implemente o padrão mestre/escravo.
Inicialmente, o processo mestre deverá enviar uma mensagem
para cada um dos escravos. Ao receberem a mensagem, os processos
escravos deverão respondê-la, enviando uma mensagem com o seu rank.
Ao receber uma mensagem de um processo escravo, o processo mestre deverá
imprimir o rank do processo escravo.
*/

int main(int argc, char** argv) {
	int size, rank;
	MPI_Status st;

	int msg = -1;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) { // Mestre
		for (int i = 1; i < size; i++)
			MPI_Send(NULL, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
		for (int i = 1; i < size; i++) {
			MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
			if (msg > 100) {
				printf("Mensagem recebida do processo %d\n", st.MPI_SOURCE);
				break;
			}
			printf("Mensagem recebida do processo %d\n", st.MPI_SOURCE);
		}
	} else { // Escravos
		MPI_Recv(NULL, 0, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		printf("Processo %d enviando..\n", rank);
		if (rank == 4) {
			msg = 500;
			MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		} else {
			MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}