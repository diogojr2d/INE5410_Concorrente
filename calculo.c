#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 5

int main(int argc, char *argv[]) {
  int i;

  double *c = (double *) malloc (sizeof(double) * SIZE);

  #pragma omp parallel for shared(c)
  for (i = 0; i < SIZE; i++) {
    c[i] = sqrt(i * 32) + sqrt(i * 16 + i * 8) + sqrt(i * 4 + i * 2 + i);
    c[i] -= sqrt(i * 32 * i * 16 + i * 4 + i * 2 + i);
    c[i] += pow(i * 32, 8) + pow(i * 16, 12);
  }

  for (i = 0; i < SIZE; i++) {
  	printf("%f ", c[i]);
  }
  printf("\n");

  free(c);

  return 0;
}