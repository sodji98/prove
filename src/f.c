// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

// compilazione: g++ -xc++ lezione5-1-counting_sort.c
//
// Obiettivo:
// 1) analisi algoritmo con distribuzione random
// ./a.out 10 -verbose

// 2) analisi algoritmo con distribuzione random sparsa (es k circa 10*n)
// ./a.out 10 -verbose

// 3) analisi algoritmo con diverse dimensioni e distribuzione random compatta e sparsa (es k circa n o 10*n)
// ./a.out 1000 -d=10 -t=100

// 3) confronto con quicksort (stesse distribuzioni random)
// g++ -xc++ lezione2-3-quicksort.c
// g++ -xc++ lezione5-1-counting_sort.c
// ./a.out 1000 -d=10 -t=100

/// intero massimo usato
#define MAX_COUNT 1000000

int ct_swap=0;
int ct_cmp=0;
int ct_opw=0;    /// conta le operazioni di scrittura su array

int max_dim=0;
int ntests=1;
int ndiv=1;
int details=0;
int graph=0;

int n=0; /// dimensione dell'array

/// file di output per grafo
ofstream output_graph;

void print_array(int* A, int dim){
    for (int j=0; j<dim; j++) {
      printf("%d ",A[j]);
    }
    printf("\n");
}




void counting_sort(int* A, int* B, int* C, int n, int k) {
  /// A: array in input 0..n-1
  /// B: array in output 0..n-1
  /// C: array per conteggi 0..k
  /// n: elementi da ordinare
  /// k: valore massimo contenuto in A

  for (int i=0;i<=k;i++){ /// reset array conteggi
    C[i]=0;
    ct_opw++;
  }

  for (int j=0;j<n;j++){ /// conteggio istogramma
    C[A[j]]++;
    ct_opw++;
  }

  if (details){
    printf("array conteggi\n");
    print_array(C,k+1);
  }
  for (int i=1;i<=k;i++){ /// C[i] contiene il numero di elementi <= i
    C[i]+=C[i-1];
    ct_opw++;
  }

  if (details){
    printf("array con conteggi accumulati\n");
    print_array(C,k+1);
  }

  for (int j=n-1;j>=0;j--){ /// per ogni elemento originale in A ->
                            /// mi chiedo nel conteggio C quanti sono gli elementi minori o uguali:
                            /// questo corrisponde alla posizione dell'elemento in B
    if (details)
      printf("A[%d]=%d, C[A[%d]]=%d --> scrivo B[%d-1]=%d\n",j,A[j],j,C[A[j]],C[A[j]],A[j]);

    B[C[A[j]]-1]=A[j];
    ct_opw++;
    C[A[j]]=C[A[j]]-1;
    ct_opw++;
  }

  if (details){
    printf("array con conteggi accumulati dopo il decremento\n");
    print_array(C,k+1);
  }



}

int parse_cmd(int argc, char **argv){
  /// controllo argomenti
  if (argc < 2) {
    printf("Usage: %s max-dim [Options]\n",argv[0]);
    printf("   max-dim: specifica la massima dimensione n del problema\n");
    printf("Options:\n");
    printf("  -d=<int>: Specifica quali dimensioni n del problema vengono lanciate in sequenza [default: 1] \n");
    printf("            n = k * max-dim / d, k=1 .. d\n");
    printf("  -t=<int>: Specifica quanti volte viene lanciato l'algoritmo per una specifica dimensione n [default: 1]\n");
    printf("            Utile nel caso in cui l'input viene inizializzato in modo random\n");
    printf("  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
    printf("  -graph: creazione file di dot con il grafo dell'esecuzione (forza d=1 t=1)\n");
    return 1;
  }

  /// parsing argomento
  max_dim = atoi(argv[1]);

  for (int i=2;i<argc;i++){
    if (argv[i][1]=='d')
      ndiv = atoi(argv[i]+3);
    if (argv[i][1]=='t')
      ntests = atoi(argv[i]+3);
    if (argv[i][1]=='v')
      details=1;
    if (argv[i][1]=='g'){
      graph=1;
      ndiv=1;
      ntests=1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  int i,test;
  int* A;
  int* B;
  int* C;
  int k; /// valore massimo nell'array di input

  if (parse_cmd(argc,argv))
    return 1;

  /// allocazione array
  A = new int[max_dim];
  B = new int[max_dim];
  C = new int[MAX_COUNT]; /// alloco l'array per i conteggi

  // init random
  srand((unsigned) time(NULL));


  if (ndiv>1)
    printf("Dim_array,N_test,min_op,avg_op,max_op,avg_case_op\n");

  //printf("Parametri: max-dim %d, d %d, t %d, verbose %d\n",max_dim,ndiv,ntests,details);

  //// inizio ciclo per calcolare ndiv dimensioni di array crescenti
  for (n = max_dim/ndiv;n<=max_dim;n+=max_dim/ndiv){
    int op_min=-1;
    int op_max=-1;
    long op_avg=0;


    //// lancio ntests volte per coprire diversi casi di input random
    for (test=0;test<ntests;test++){

      /// inizializzazione array: numeri random con range dimensione array
      k=0;

      for (i=0; i<n; i++) {
	//A[i]= n-i;
	A[i]= rand() % (n*1);

	if (k<A[i]) k=A[i];  /// memorizzo il massimo in k
      }

      /// controllo se il massimo valore e' troppo grande
      if (k>=MAX_COUNT){
	printf("interno in array troppo grande\n");
	return -1;
      }

      if (details){
	printf("creato array di dimensione %d\n",n);
	print_array(A,n);
      }

      ct_opw=0;

      /// algoritmo di sorting
      counting_sort(A,B,C,n,k);

      if (details){
	printf("array ordinato\n");
	print_array(B,n);
      }


      op_avg+=ct_opw;
      if (op_min<0 || op_min>ct_opw) op_min=ct_opw;
      if (op_max<0 || op_max<ct_opw) op_max=ct_opw;

    }

    if (ndiv>1)
      printf("%d,%d,%d,%.1f,%d,%.1f\n",
	     n,ntests,
	     op_min ,(0.0+op_avg)/ntests ,op_max ,.0+n+k);


  } /// termine del loop che lavora con array di dimensione n


  delete [] A;
  delete [] B;
  delete [] C;  /// dealloco l'array di supporto per i conteggi (dipende da k = massimo nell'array in uso)

  return 0;
}
