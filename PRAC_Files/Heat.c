#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, atoi

// Define here as constants for easy change
#define REAL float
#define HEAT_COEFF 0.001234

void init ( REAL *In, int N )
{ 
  In  [0]   = 1.0;
  In [N/3]  = 8.0;
  In[4*N/7] = 3.0;
  In [N+1]  = 9.0;
}

void PrintCheck ( REAL *In, int N )
{
  REAL S= 0.0;
  int x;
  if (N > 100)
  {
    for (x=0; x<N; x++) // compute checksum of final state 
      S = S + In[x];
    printf("\nCheckSum = %2.12f\n", S);
  }
  else  
    for (x=0; x<N; x++) // print final state
      printf("%2.12f\n", In[x]);
}


void Heat_V1 (REAL *Uin, REAL *Uout, int N, int T)
{
  for (int t=0; t<T; t++)  // loop on time
  {
    Uout[0] = Uin[0];           // Copy Left boundary

    for (int x=1; x<=N; x++)
      Uout[x] = Uin[x] + HEAT_COEFF*(Uin[x-1] + Uin[x+1] - 2.0*Uin[x]);

    Uout[N+1] = Uin[N+1];       // Copy Right boundary

    for (int x=0; x<=N+1; x++)
      Uin[x] = Uout[x];
  }
}


int main(int argc, char **argv)
{
  int N= 100000, T= 10000, Opt= 1;

  if (argc>1) { N  = atoi(argv[1]); } // get  first command line parameter
  if (argc>2) { T  = atoi(argv[2]); } // get second command line parameter
  if (argc>3) { Opt= atoi(argv[3]); } // get third command line parameter
 
  if (N < 1 || T < 1) {
    printf("arguments: N T (N: vector size, T: simulation steps)\n");
    return 1;
  }

  REAL *Uin = (REAL *) calloc ( (N+2), sizeof(REAL) );
  REAL *Uout= (REAL *) calloc ( (N+2), sizeof(REAL) );
  if (!Uin || !Uout) { printf("Cannot allocate vectors\n"); exit(1); }

  printf("Heat Diffusion (C): N= %d and T= %d\n", N, T);

  init( Uin, N );

  switch(Opt) 
  {
    case 1: 
      printf("**************** VERSION 1 *****************\n");
      Heat_V1 (Uin, Uout, N, T);
      break;

    case 2: 
      printf("**************** VERSION 2 *****************\n");
      break;
  }

  PrintCheck ( Uin, N+2 );

  free(Uin); free(Uout);
}

