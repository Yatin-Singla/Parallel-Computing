/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *	
// Computing the sum of n numbers in parallel

    Hypercubic permutation - MyAllReduce
    Simple Array bus permutation - NaiveAllReduce
    STD MPI_Allreduce - MPILibraryAllReduce

// Assumptions: p is a power of 2
// 				n >> p and n divisible by p
// 				value of n passed as argument to the program 
// 				code will auto-generate n/p elements within each proc using a rand function.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include <assert.h>
#include <time.h>

int rank, p, n;

// hypercubic permutation
// returns global sum
int MyAllReduce(void);

// Array bus permutation
// returns global sum
int NaiveAllReduce(void);

// performs STD MPI REDUCE
// returns global sum
int MPILibraryAllReduce(void);


// generates an array of size n/p of random ints
// returns memory address of the newly created array 
int * GenerateArray(int size);

// passes a newly generated array A as input to all three Reduce fundtions
// to perform reduction
// returns 1 if correctness test fails
// returns 0 for pass
int testCorrectness(void);

// compares runtime performance for all three reduce functions
// and generates a log file
void pComparison(void);

// prints array of given size
void printArray(int * inArr, int size);

int main(int argc,char *argv[]) {

    // Initialize MPI
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   if(argc!=2) {
   	printf("Usage: sum <n>\n");
     MPI_Finalize();
     return(1);
   }

   n = atoi(argv[1]);
   if(rank==0) {
   	printf("Info: input size n = %d\n",n);
   }

   // precondition is divisible by p
   assert(n%p==0);
  
   int * arr = GenerateArray(n/p);
   printArray(arr, n/p);   

   MPI_Finalize();
}

int * GenerateArray(int size){
    //1. generate local array of size of n/p
    int *a=malloc(sizeof(int)*size);

    // make sure memory was allocated
    assert(a!=NULL);

	srand48(time(NULL)+rank);

	for(int i = 0; i < size; i++) {
		a[i] = drand48()*100;
	}

    return a;
}


void printArray(int * inArr, int size) {
     printf("Rank = %d\n", rank);
     printf("------------------ARRAY--------------------\n\n|");
     // size = n/p
     for (int i = 0; i < size; i++) { 
        printf(" %d,", inArr[i]);
     } 
     printf("|\n\n");
     return;
}
