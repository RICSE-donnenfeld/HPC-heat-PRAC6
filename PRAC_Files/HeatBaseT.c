#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, atoi

// Define here as constants for easy change
#define REAL float
#define HEAT_COEFF 0.001234

REAL ** Create_2D_array ( int D1, int D2 )
{
  // Dynamically create 2D matrix as a an array of arrays
  REAL ** V  = (REAL **) malloc ( D1*sizeof(REAL*) );
  if (!V) { printf("Cannot allocate vector\n"); exit(1); }

  for (int i=0; i<D1; i++)
  {
    V[i] = (REAL *) calloc ( D2, sizeof(REAL) );
    if (!V) { printf("Cannot allocate vector\n"); exit(1); }
  }
  return V;
}

void Free_2D_array ( REAL **V, int D1 )
{
  for (int i=0; i<D1; i++)
    free( V[i] );
  free(V);
}

void init ( REAL **V, int N )
{ 
  V[0]  [0]  = 1.0;
  V[0] [N/3] = 8.0;
  V[0][4*N/7]= 3.0;
  V[0] [N+1] = 9.0;
}

void PrintCheck ( REAL **V, int N, int T )
{
  REAL S= 0.0;
  int x;
  if (N > 100)
  {
    for (x=0; x<N; x++) // compute checksum of final state 
      S = S + V[T][x];
    printf("\nCheckSum = %2.12f\n", S);
  }
  else  
    for (x=0; x<N; x++) // print final state
      printf("%2.12f\n", V[T][x]);
}


int main(int argc, char **argv)
{
  int  x, t, N= 100000, T=10000;

  if (argc>1) { N = atoi(argv[1]); } // get  first command line parameter
  if (argc>2) { T = atoi(argv[2]); } // get second command line parameter
 
  if (N < 1 || T < 1) {
    printf("arguments: N T (N: vector size, T: simulation steps)\n");
    return 1;
  }

  REAL **U = Create_2D_array( T+1, N+2 );

  printf("Heat Diffusion (C): N= %d and T= %d\n", N, T);

  init( U, N );

  for (t=0; t<T; t++)  // loop on time
  {
    U[t+1][0] = U[t][0];           // Copy Left boundary

    for (x=1; x<=N; x++)                   // loop on X dimension
      U[t+1][x] = U[t][x] +  HEAT_COEFF*(U[t][x-1] + U[t][x+1] - 2.0*U[t][x]);

    U[t+1][N+1] = U[t][N+1];    // Copy Right boundary
  }

  PrintCheck ( U, N+2, T );
  Free_2D_array( U, T+1 );
}

