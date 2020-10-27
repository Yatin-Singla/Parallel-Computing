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
#include <stdbool.h>
#include <sys/time.h>

int rank, p, n;

// hypercubic permutation
// returns global sum
int MyAllReduce(int * inArr, int size, MPI_Op op);

// Array bus permutation
// returns global sum
int NaiveAllReduce(int * inArr, int size, MPI_Op op);

// performs STD MPI REDUCE
// returns global sum
int MPILibraryAllReduce(int * inArr, int size, MPI_Op op);

// generates an array of size n/p of random ints
// returns memory address of the newly created array 
int * GenerateArray(int size);

// passes a newly generated array A as input to all three Reduce fundtions
// to perform reduction
// returns 1 if correctness test fails
// returns 0 for pass
bool testCorrectness(void);

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
   
   /*
   bool isComplete = true;

   isComplete = testCorrectness();
  
   if (rank == 0) {
        printf("isComplete = %d", isComplete);
   } 
   */

   pComparison();

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

int MyAllReduce(int * inArr, int size, MPI_Op op) {

   int localMax = inArr[0];
   int localSum = 0;
   if (op == MPI_SUM) {
      //2. compute local sum
      for(int i = 0; i < size; i++) {
      	localSum += inArr[i];
      }
      if (p == 1) {
        printf("Rank=%d: MyAllReduce Global Sum = %d\n",rank,localSum);
        return localSum;
      }
   }
   // else op == MPI_MAX
   else {
     for (int i = 0; i < size; i++) {
         if (inArr[i] > localMax) {
             localMax = inArr[i];
         }
     }
     if (p == 1) {
        printf("Rank=%d: MyAllReduce Global Max = %d\n",rank,localMax);
        return localMax;
     }
   }
   
   //3. compute sum in parallel
   int globalSum = localSum, receivedSum = 0;
   int globalMax = localMax, receivedMax = 0;
   int temp = 1, iter = 0;

   // : there will be lg p iterations
   for(int step = 1; step < p; step *= 2) {
   		if(rank==0) {
			printf("Info: Iteration #%d\n",iter);
			//fflush(stdout);
		}
   	
        // : during iteration i, rank j will communicate with rank j'
        // :			iff the i^th lsb(j) <is *not* equal to> i^th lsb(j')
   		int mate = rank^temp;  
	    //	printf("Rank=%d: Mate =%d\n",rank,mate);
	    //	fflush(stdout);

		MPI_Status stat;
        if (op == MPI_SUM) {
            // : for the individual communications let us use the MPI_Sendrecv primitive
		    MPI_Sendrecv(&globalSum, 1, MPI_INT, mate, 0, &receivedSum, 1, MPI_INT,\
		    			 mate, 0, MPI_COMM_WORLD, &stat);
		    globalSum += receivedSum;
        }
        else {
		    MPI_Sendrecv(&globalMax, 1, MPI_INT, mate, 0, &receivedMax, 1, MPI_INT,\
		    			 mate, 0, MPI_COMM_WORLD, &stat);
            if (receivedMax > globalMax) {
                globalMax = receivedMax;
            }
        }
	    //printf("Rank=%d: iter<%d>: rec'd message from %d\n",rank,iter,stat.MPI_SOURCE);
	    //fflush(stdout);
		temp = temp<<1;
		iter++;

		MPI_Barrier(MPI_COMM_WORLD);
   }
  
  if (op == MPI_SUM) { 
      printf("Rank=%d: MyAllReduce Global Sum = %d\n",rank,globalSum);
      return globalSum;
  }
  else {
      printf("Rank=%d: MyAllReduce Global Max = %d\n",rank,globalMax);
      return globalMax; 
  }
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

int NaiveAllReduce(int * inArr, int size, MPI_Op op) {
    int source = rank -1, dest = rank +1; 
    MPI_Status status;
    if (op == MPI_SUM) {
        int localSum = 0;
        //2. compute local sum
        for(int i = 0; i < size; i++) {
       	    localSum += inArr[i];
        }
        int globalSum = localSum, receivedSum = 0;
    
        if (rank == 0) {
            dest = dest % p;
            //MPI send localMax
            MPI_Send(&globalSum, 1, MPI_INT, dest, rank, MPI_COMM_WORLD);
            //recv Global max after computation
            MPI_Recv(&receivedSum, 1, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            globalSum = receivedSum;
        }
        else if (rank > 0 && rank < p-1) { 
            // MPI recv and send
            MPI_Recv(&receivedSum, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            globalSum += receivedSum;
            MPI_Send(&globalSum, 1, MPI_INT, dest, rank, MPI_COMM_WORLD);


            //recv globalMax after computation
            MPI_Recv(&receivedSum, 1, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            globalSum = receivedSum;
            //relay the information
            MPI_Send(&globalSum, 1, MPI_INT, source, rank, MPI_COMM_WORLD);
        }
        // rank == p-1
        else {
            source = source % p;
            // MPI recv
            MPI_Recv(&receivedSum, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            globalSum += receivedSum;
            // relay the globalMax
            MPI_Send(&globalSum, 1, MPI_INT, source, rank, MPI_COMM_WORLD);
        }
        printf("Rank=%d: Naive Reduce Global Sum = %d\n",rank,globalSum);
        return globalSum;
    }
    // else op == MPI_MAX
    else {
        int localMax = inArr[0];
        for (int i = 0; i < size; i++) {
              if (inArr[i] > localMax) {
                  localMax = inArr[i];
            }
        }
        int globalMax = localMax, receivedMax = 0;

        if (rank == 0) {
            dest = dest % p;
            //MPI send localMax
            MPI_Send(&globalMax, 1, MPI_INT, dest, rank, MPI_COMM_WORLD);
            //recv Global max after computation
            MPI_Recv(&receivedMax, 1, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            globalMax = receivedMax;
        }
        else if (rank > 0 && rank < p-1) { 
            // MPI recv and send
            MPI_Recv(&receivedMax, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (receivedMax > globalMax) {
                globalMax = receivedMax;
            }
            MPI_Send(&globalMax, 1, MPI_INT, dest, rank, MPI_COMM_WORLD);


            //recv globalMax after computation
            MPI_Recv(&receivedMax, 1, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            globalMax = receivedMax;
            //relay the information
            MPI_Send(&globalMax, 1, MPI_INT, source, rank, MPI_COMM_WORLD);
        }
        // rank == p-1
        else {
            source = source % p;
            // MPI recv
            MPI_Recv(&receivedMax, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (receivedMax > globalMax) {
                globalMax = receivedMax;
            }
            // relay the globalMax
            MPI_Send(&globalMax, 1, MPI_INT, source, rank, MPI_COMM_WORLD);
        }

        printf("Rank=%d: Naive Reduce Global Max = %d\n",rank,globalMax);
        return globalMax;
    }
}

int MPILibraryAllReduce(int * inArr, int size, MPI_Op op) {
  if (op == MPI_MAX) {
        // compute localMax 
        int localMax = inArr[0];
        for (int i = 0; i < size; i++) {
            if (inArr[i] > localMax) {
                localMax = inArr[i];
            }
        }
        int globalMax = localMax;
        int receivedMax = localMax;
        MPI_Allreduce(&localMax, &receivedMax, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        if (receivedMax > globalMax) {
            globalMax = receivedMax;
        }
        printf("Rank=%d: MPI Library All Reduce Global Max = %d\n",rank,globalMax);
        return globalMax;
  }
  // op == MPI_SUM
  else {
      //2. compute local sum
      int localSum = 0;
      for(int i = 0; i < size; i++) {
      	localSum += inArr[i];
      }
      int globalSum = 0;
      //3. compute sum in parallel
      int receivedSum = 0;
      MPI_Allreduce(&localSum, &receivedSum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
      globalSum += receivedSum;
      printf("Rank=%d: MPI Library All Reduce Global Sum = %d\n",rank,globalSum);
      return globalSum;  
  }
}


bool testCorrectness(void) {

   int MyAllRedGS = 0, MyAllRedGM = 0;
   int NaiveAllRedGS = 0, NaiveAllRedGM = 0;
   int MPILibAllRedGS = 0, MPILibAllRedGM = 0;

   int * arr = GenerateArray(n/p);
   printArray(arr, n/p);   

   MyAllRedGS = MyAllReduce(arr, n/p, MPI_SUM);
   MyAllRedGM = MyAllReduce(arr, n/p, MPI_MAX);
   NaiveAllRedGS = NaiveAllReduce(arr, n/p, MPI_SUM);
   NaiveAllRedGM = NaiveAllReduce(arr, n/p, MPI_MAX);
   MPILibAllRedGS = MPILibraryAllReduce(arr, n/p, MPI_SUM);
   MPILibAllRedGM = MPILibraryAllReduce(arr, n/p, MPI_MAX);

   
   if (rank == 0 ) { 
        printf("My All Reduce Global Sum = %d\n", MyAllRedGS);
        printf("My All Reduce Global Max = %d\n", MyAllRedGM);
        printf("Naive All Reduce Global Sum = %d\n", NaiveAllRedGS);
        printf("Naive All Reduce Global Max = %d\n", NaiveAllRedGM);
        printf("MPI Library All Reduce Global Sum = %d\n", MPILibAllRedGS);
        printf("MPI Library All Reduce Global Max = %d\n", MPILibAllRedGM);

        if ((MyAllRedGS == NaiveAllRedGS) && (MyAllRedGS == MPILibAllRedGS) && (MyAllRedGM == NaiveAllRedGM) && (MyAllRedGM == MPILibAllRedGM)) {
             printf("True\n");
             return true;
        }
   }

   return false;
   
}

void pComparison(void) {
    FILE * outfile = fopen("log.csv", "a");
    int myReduce = 0, naiveReduce = 0, mpiReduce = 0;
    double myReduceRuntime = 0.0, naiveReduceRuntime = 0.0, mpiReduceRuntime = 0.0;
    struct timeval myReduceStart, myReduceStop, naiveReduceStart, naiveReduceStop, mpiReduceStart, mpiReduceStop;

    int * a = NULL;

    /*
    if (rank == 0) {
        fprintf(outfile, "p, N, MyAllReduce, NaiveReduce, MPILibraryReduce\n");
    }
    */

    a = GenerateArray(n/p);

    // making a dummy call to avoid reporting communication overhead in the results
    MyAllReduce(a, n/p, MPI_SUM);
    
    gettimeofday(&myReduceStart, NULL);
    MyAllReduce(a, n/p, MPI_SUM);
    gettimeofday(&myReduceStop, NULL);
    myReduceRuntime = (myReduceStop.tv_sec - myReduceStart.tv_sec)*1000000 + (myReduceStop.tv_usec - myReduceStart.tv_usec);

    gettimeofday(&naiveReduceStart, NULL);
    NaiveAllReduce(a, n/p, MPI_SUM);
    gettimeofday(&naiveReduceStop, NULL);
    naiveReduceRuntime = (naiveReduceStop.tv_sec - naiveReduceStart.tv_sec)*1000000 + (naiveReduceStop.tv_usec - naiveReduceStart.tv_usec);

    gettimeofday(&mpiReduceStart, NULL);
    MPILibraryAllReduce(a, n/p, MPI_SUM);
    gettimeofday(&mpiReduceStop, NULL);
    mpiReduceRuntime = (mpiReduceStop.tv_sec - mpiReduceStart.tv_sec)*1000000 + (mpiReduceStop.tv_usec - mpiReduceStart.tv_usec);
    
    if (rank == 0) {
        // N in KB
        fprintf(outfile, "%d, %d, %lf, %lf, %lf\n", p, (int)((n*4)/1024), myReduceRuntime, naiveReduceRuntime, mpiReduceRuntime);
    }
    free(a);

    fclose(outfile);
    return;
}
