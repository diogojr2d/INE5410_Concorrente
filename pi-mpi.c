#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

unsigned int compute_pi(unsigned int, unsigned int);

int main(int argc, char **argv){
  unsigned int pontos;
  unsigned int pontos_no_circulo = 0;

  int size, rank;
  unsigned int msg;
  MPI_Status st;

  if(argc != 2){
    printf("Uso:\n");
    printf("\t%s <numero de pontos a serem sorteados>\n", argv[0]);
    return 1;
  }
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) { // Mestre
    pontos = atoi(argv[1]);

    // divide quantos pontos cada processo vai calcular
    unsigned int ppp = pontos/(size-1);
    msg = ppp;
    for (int i = 1; i < size; i++) {
      MPI_Send(&msg, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD);
    }

    for (int i = 1; i < size; i++) {
      MPI_Recv(&msg, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
      pontos_no_circulo += msg;
    }    

    // calcula a aproximacao de Pi baseado nos pontos sorteados
    printf("Pi = %.040f\n", ((double)pontos_no_circulo/(double)pontos)*4);
  }
  else { // Escravos
    MPI_Recv(&msg, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
    pontos_no_circulo = compute_pi(0, msg);
    msg = pontos_no_circulo;
    MPI_Send(&msg, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}

unsigned int compute_pi(unsigned int seed, unsigned int pontos){
  unsigned int i;
  unsigned int pontos_no_circulo;
  double x, y;

  pontos_no_circulo = 0;
  srand(seed);
  
  for(i=0; i<pontos; i++){
  	// sorteia um ponto: coordenadas x e y dentro do quadrado
  	// consideramos que R = 1, então x e y pertencem ao intervalo [0; 1]
    x = (double)rand()/(double)(RAND_MAX);
    y = (double)rand()/(double)(RAND_MAX);      
    
    // verifica se o ponto sorteado encontra-se dentro do circulo
    // um ponto (x, y) esta dentro do circulo se: x^2 + y^2 < R^2
    // nesse caso, consideramos R = 1
    if( (x*x + y*y) < 1 ){
      pontos_no_circulo++;
    }      
  }
  
  return pontos_no_circulo;
}
