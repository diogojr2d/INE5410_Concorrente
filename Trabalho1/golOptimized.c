/*
* The Game of Life
*
* a cell is born, if it has exactly three neighbours
* a cell dies of loneliness, if it has less than two neighbours
* a cell dies of overcrowding, if it has more than three neighbours
* a cell survives to the next generation, if it does not die of loneliness
* or overcrowding
*
* In this version, a 2D array of ints is used.  A 1 cell is on, a 0 cell is off.
* The game plays a number of steps (given by the input), printing to the screen each time.  'x' printed
* means on, space means off.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned char cell_t;
int numThreads = 4;
int currentStep = 0;
cell_t** prev;
cell_t** next;
cell_t** tmp;
pthread_barrier_t barrier;

cell_t ** allocate_board (int size) {
  cell_t ** board = (cell_t **) malloc(sizeof(cell_t*)*size);
  int	i;
  for (i=0; i<size; i++)
    board[i] = (cell_t *) malloc(sizeof(cell_t)*size);
  return board;
}

void free_board (cell_t ** board, int size) {
  int     i;
  for (i=0; i<size; i++)
  	free(board[i]);
  free(board);
}


/* return the number of on cells adjacent to the i,j cell */
int adjacent_to (int size, int i, int j) {
  int	k, l, count=0;

  int sk = i-1;
  int ek = i+1;
  int sl = j-1;
  int el = j+1;

  for (k=sk; k<=ek; k++) {
  	for (l=sl; l<=el; l++) {
  		count+=prev[k][l];
  	}
  }
  count-=prev[i][j];

  return count;
}

void play (int size, int start, int end) {
  int	i, j, a, c;
  /* for each cell, apply the rules of Life */

  for (c=start; c<end; ++c) {
    i = c%size + 1;
    j = c/size + 1;
    a = adjacent_to (size, i, j);
    if (a == 2) next[i][j] = prev[i][j];
    if (a == 3) next[i][j] = 1;
    if (a < 2) next[i][j] = 0;
    if (a > 3) next[i][j] = 0;
  }
}

/* print the life board */
void print (cell_t ** board, int size) {
  int	i, j;
  /* for each row */
  for (j=0; j<size; j++) {
    /* print each column position... */
    for (i=0; i<size; i++)
    printf ("%c", board[i+1][j+1] ? 'x' : ' ');
    /* followed by a carriage return */
    printf ("\n");
  }
}

/* read a file into the life board */
void read_file (FILE * f, cell_t ** board, int size) {
  int	i, j;
  char	*s = (char *) malloc(size+10);

  /* read the first new line (it will be ignored) */
  fgets (s, size+10,f);

  j = 0;
  for (i = 0; i < size; i++)
    board[i][j] = 0;

  j = size-1;
  for (i = 0; i < size; i++)
    board[i][j] = 0;

  i = 0;
  for (j = 0; j < size; j++)
    board[i][j] = 0;

  i = size-1;
  for (j = 0; j < size; j++)
    board[i][j] = 0;

  /* read the life board */
  for (j=1; j<size-1; j++) {
    /* get a string */
    fgets (s, size+10,f);
    /* copy the string to the life board */
    for (i=1; i<size-1; i++)
    	board[i][j] = s[i-1] == 'x';
  }
}

void* thrWork(void* args_) {
  int* args = (int*)args_;
  int thrID = args[0];
  int thrRange = args[1];
  int size = args[2];
  int steps = args[3];

  // Calcula células que serão atualizadas por cada thread
  int start = thrID * thrRange;
  int end = start + thrRange;
  if (thrID == numThreads - 1) {
    end += (size*size)%numThreads;
  }

  int s = 0;
  while (currentStep < steps) {
    s = 0;
    play (size, start, end);  

    // aguarda que todas as threads calculem o novo tabuleiro
    s = pthread_barrier_wait(&barrier);

    // apenas uma thread atualiza e imprime o tabuleiro novo
    if (s == PTHREAD_BARRIER_SERIAL_THREAD) {
      #ifdef DEBUG
        printf("%d ----------\n", currentStep + 1);
        print (next,size);
      #endif
      tmp = next;
      next = prev;
      prev = tmp;
      currentStep++;
    }

    // aguarda a atualização, antes de partir para o próximo step
    pthread_barrier_wait(&barrier);
  }
  pthread_exit(NULL);
}

int main (int argc, char** argv) {
  // Verifica argumento (número de threads)
  if (argc == 2) {
    numThreads = atoi(argv[1]);
    if (numThreads < 1) {
      printf("Erro: numero de threads deve ser positivo.\n");
      return 0;
    }
  }

  // Lê arquivo e aloca tabuleiros
  int size, steps;
  FILE    *f;
  f = stdin;
  fscanf(f,"%d %d", &size, &steps);
  prev = allocate_board (size+2);
  read_file (f, prev,size+2);
  fclose(f);
  next = allocate_board (size+2);
  
  // Imprime passos intermediários
  #ifdef DEBUG
  printf("Initial:\n");
  print(prev,size);
  #endif

  // Inicializa threads
  int thrRange = size*size/numThreads;
  pthread_t threads[numThreads];
  pthread_barrier_init(&barrier, NULL, numThreads);
  int i;
  for (i = 0; i < numThreads; ++i) {
    int* args = malloc(sizeof(int)*4);
    args[0] = i;
    args[1] = thrRange;
    args[2] = size;
    args[3] = steps;
    pthread_create(&threads[i], NULL, thrWork, (void*)args);
  }

  int i2;
  for (i2 = 0; i2 < numThreads; ++i2) {
    pthread_join(threads[i2], NULL);
  }

  // Imprime resultado
  #ifdef RESULT
    printf("Final:\n");
    print (prev,size);
  #endif

  free_board(prev,size);
  free_board(next,size);
}
