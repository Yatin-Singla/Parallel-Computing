#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <assert.h>
#include <time.h>
#include <math.h>

int N, G;
int rank,p;
time_t t;

// function declaration
void printLocalBlock(int * array, int row, int col);
int * GenerateInitialGoL(int row, int col);
void print_matrix(int **m);
// Need memory access to prev generation array 
// to make changes to it
void Simulate(int ** prevGenArr);
void DisplayGoL(int * blockArray);
//void freeMatrix(int ** array, int row);
//void printVectorMatrix(int *vector, int rows, int cols);

int main(int argc,char *argv[]) {
   struct timeval t1,t2;

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   srand((unsigned) time(&t));
   printf("my rank=%d\n",rank);
   printf("Rank=%d: number of processes =%d\n",rank,p);

   int * array = NULL;

   if (argc >= 2) {
      N = atoi(argv[1]);
      G = atoi(argv[2]);
      printf("Debug: Size of matrix = %d\n", N);
      printf("Debug: Generations = %d\n", G);
   }

   // array is editable and has the original copy
   // of allocated memory nby generate initial game of life
   array = GenerateInitialGoL((int)N/p, N);
   printLocalBlock(array, (int)N/p, N);
    
   DisplayGoL(array);
   //Simulate(&array);
   // int ** bigMatrix = GenerateInitialGoL(N,N);

    //MPI_Gather(array, (N* ((int)N/p)), MPI_INT, bigMatrix, (N * ((int)N/p)), MPI_INT, 0, MPI_COMM_WORLD);
   
   //printLocalBlock(bigMatrix, N, N);
   //free(array);
   //free(bigMatrix);
/*
    //initialize buffers
    char *x = (char *) calloc (1024*1025*maxSize, sizeof(char));
    char *y = (char *) calloc (1024*1025*maxSize, sizeof(char));

    // time variable declaration
    double tSend, tRecv;
    // initalize outfiles
    FILE * SendOutfile = fopen("blockSend.txt", "w");
    FILE * RecvOutfile = fopen("blockRecv.txt", "w");
    
    int destRank = 0;
    
	MPI_Status status;
    // loop sends data from 1Byte -> maxSize MB (multiply by 1024*1024)
    // send size is a number but unit of data is sent in byte
   for(int sendSize = 1; sendSize <= maxSize*1024*1024; sendSize *= 2){
 
           // process 1 is the sender
            if(rank==1) {
                //x = (char *) calloc (sendSize, sizeof(char));
                gettimeofday(&t1,NULL);
                for (int i = 0; i < loops; i++) {
                    MPI_Send(x,sendSize,MPI_BYTE,destRank,0,MPI_COMM_WORLD);
                }
                gettimeofday(&t2,NULL);
                tSend = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
                fprintf(SendOutfile,"Rank=%d: sent message size %d B; Send time %.2f microsec\n", rank,sendSize,(double)tSend/loops);
                //free(x);
                //x = NULL;
            }
            else if (rank==0) {
                //y = (char *) calloc (sendSize,sizeof(char));
                gettimeofday(&t1,NULL);
                for(int i = 0; i < loops; i++) {
                    MPI_Recv(y,sendSize,MPI_BYTE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                }
                gettimeofday(&t2,NULL);
                tRecv = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);      
                fprintf(RecvOutfile,"Rank=%d: received message size %d B; Recv time %.2f microsec\n",rank,sendSize,(double)tRecv/loops);
                //free(y);
                //y = NULL;
            }

  }

    fclose(SendOutfile);
    fclose(RecvOutfile);
    */
   MPI_Finalize();
   return 0;
}

// initializes only a block of matrix
// row == int N/p
// col == N
int * GenerateInitialGoL(int row, int col){
    int i = 0, j = 0;
    // declare N/p rows
    int *m = (int *) malloc(row*col * sizeof(int));
    
    // declare N columns
    /*for(i = 0; i < row; i++){
        m[i] = malloc(col * sizeof(int));
    }*/

    // initialize the block
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            int seed = rank+1;
            seed = seed*i;
            // even == alive 
            // odd == dead 
            m[i*col + j] = rand_r(&seed) % 10; 
        }
    }

    return m;
}

void printLocalBlock(int * array, int row, int col){

    printf("Rank = %d\n", rank);
    printf("-------------------MATRIX--------------------\n\n");
    // row = int N/p
    // col = N
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
                if(j==0){
                    printf("|");
                }
                printf(" %d ", array[i*col + j]);
        }
        printf("|\n");
    }
    printf("\n");
}


void Simulate(int ** prevGenArr) {
    int * nextGenArr = GenerateInitialGoL((int)N/p, N);
    int * prevRow = (int *) malloc(sizeof(int)*N);
    int * nextRow = (int *) malloc(sizeof(int)*N);
    int prev = rank - 1, next = rank + 1;
    MPI_Status statusTop, statusBottom;

    // simulation
    for(int g=0; g < G; g++){
        // ensures all processes are excuting
        // the same generation at any given time.
        MPI_Barrier(MPI_COMM_WORLD);

        // send and recv
        if (rank == 0) {
            prev = p-1;
        }
        else if (rank == (p-1)) {
            next = 0;
        }

        //if (rank==0) 
          //  printf("prevGenArr last row 3 col= %d\n", (*prevGenArr)[((int)N/p)-1][3]);
        // send top row
        if (rank!=0) {
            printLocalBlock(*prevGenArr, (int) N/p, N);
            MPI_Send((*prevGenArr), N, MPI_INT, prev, rank, MPI_COMM_WORLD);
        }
        // send bottom row
        //MPI_Send(&(prevGenArr[((int)N/p)-1]), N, MPI_INT, next, rank, MPI_COMM_WORLD);

        // recv top row
        if (rank==0) {
            printf("print next row for process %d \n\n", rank);
            for (int i = 0; i < N; i++)
                    printf(" %d ", nextRow[i]);
            *nextRow = 1;
            *(nextRow +1) = 2;
            MPI_Recv(nextRow, N, MPI_INT, prev, MPI_ANY_TAG, MPI_COMM_WORLD, &statusTop);
            printf("count recvd = %ld\n MPI_SOURCE = %d\n MPI_TAG = %d\n MPI_ERR=%d\n", statusTop._ucount, statusTop.MPI_SOURCE, statusTop.MPI_TAG, statusTop.MPI_ERROR);
            printf("next row after edit for process %d \n\n", rank);
            for (int i = 0; i < N; i++)
                    printf(" %d ", nextRow[i]);
        }
        // recv bottom row
        //MPI_Recv(nextRow, N, MPI_INT, next, MPI_ANY_TAG, MPI_COMM_WORLD, &statusBottom);

        
        // simulate new generation
       /* if (rank == 0) {
            printf("rank 0 top Row \n\n|");
            for(int i = 0; i < N; i++) {
                printf(" %d ", prevRow[i]);
            }
            printf("|\n");
            printf("rank 0 bottom Row \n\n|");
            for(int i = 0; i < N; i++) {
                printf(" %d ", nextRow[i]);
            }
            printf("|\n");
        }
*/
        // display new generation
    }
    free(prevRow);
    free(nextRow);
    free(nextGenArr);
}
/*
void printVectorMatrix(int *vector, int rows, int cols) {
    printf("Rank = %d\n", rank);
    printf("-------------------------VECTOR----------------------\n\n");
    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == 0) {
                    printf("|");
            }
            printf(" %d ", vector[i*cols + j]); 
        }
        printf("|\n");
    }
    printf("\n\n");
    return;
}
*/
void DisplayGoL(int * blockArray){
    // generate nxn matrix to accumulate all the data in
    int * matrix;
    int root = 0;
   
    if(rank == root){
        matrix = (int *)malloc(N*N* sizeof(int));
    }

    MPI_Gather(blockArray, (N* ((int)N/p)), MPI_INT, matrix, (N * ((int)N/p)), MPI_INT, root, MPI_COMM_WORLD);
    
    if (rank == root) {
        printLocalBlock(matrix, N, N);
        free(matrix);
    }
     
    return;
}

/*
// deallocate 2D matrix
void freeMatrix(int ** array, int row){
    for (int i = 0; i < row; i++) {
       free(array[i]); 
    }
    free(array);
    return;
}
*/
