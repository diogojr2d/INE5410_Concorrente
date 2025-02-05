#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long long unsigned int fat(int n)
{
    long long unsigned int res;
    int i;
    res = 1;

    #pragma omp parallel for reduction(*:res)
    for(i = 2; i <= n; i++) {
      res *= i;
    }
    return res;
}

int main(int argc, char **argv)
{
  int n;
  long long unsigned int resultado;
  if(argc<2){
    printf("uso ./fatorial <numero natural>\n");
    exit(1);
  }
  n = atoi(argv[1]);
  if(n < 0){
    printf("Erro! Numero de entrada nao e' natural\n");
    exit(1);
  }

  printf("Calculando fatorial de %d sem OpenMP\n",n);

  resultado = fat(n);
  printf("fatorial(%d) = %llu\n", n, resultado);

  return 0;
}
