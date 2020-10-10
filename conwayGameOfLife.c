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
// addditionally I want to pass by reference
void Simulate(int ** prevGenArr);
void DisplayGoL(int * blockArray);
void ComputeGen(int ** lastGen, int * topRow, int * bottomRow, int ** newGen);

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
    
   Simulate(&array);
   
   free(array);
/*
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
// can't generate a dynamically allocated 2D matrix
// as it won't be contiguous in memory 
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
    int * topRow = (int *) malloc(sizeof(int)*N);
    int * bottomRow = (int *) malloc(sizeof(int)*N);
    int prevRank = rank - 1, nextRank = rank + 1;
    MPI_Status statusTop, statusBottom;

    // simulation
    for(int g=0; g < G; g++){
        // ensures all processes are excuting
        // the same generation at any given time.
        MPI_Barrier(MPI_COMM_WORLD);

        // send and recv
        if (rank == 0) {
            prevRank = p-1;
        }
        else if (rank == (p-1)) {
            nextRank = 0;
        }

        // SEND AND RECEIVE ROWS
        // send top row to prevRank and recv bottom row from next rank
        MPI_Sendrecv(*prevGenArr, N, MPI_INT, prevRank, rank, bottomRow, N, MPI_INT, nextRank, MPI_ANY_TAG, MPI_COMM_WORLD, &statusBottom);
        // send bottom row to next rank and recv top row from prev rank
        MPI_Sendrecv(((*prevGenArr)+ (N*(((int)N/p)-1))), N, MPI_INT, prevRank, rank, topRow, N, MPI_INT, nextRank, MPI_ANY_TAG, MPI_COMM_WORLD, &statusBottom);
//        printf("count recvd = %ld\n MPI_SOURCE = %d\n MPI_TAG = %d\n MPI_ERR=%d\n", statusTop._ucount, statusTop.MPI_SOURCE, statusTop.MPI_TAG, statusTop.MPI_ERROR);
      //  printf("print top row for rank %d\n", rank);
      //  printLocalBlock(topRow, 1, N);
      //  printf("print bottom row for rank %d\n", rank);
      //  printLocalBlock(bottomRow, 1, N);

        ComputeGen(prevGenArr, topRow, bottomRow, &nextGenArr);
        // display new generation
    }
    free(topRow);
    free(bottomRow);
    free(nextGenArr);
}

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

void ComputeGen(int ** lastGen, int * topRow, int * bottomRow, int ** newGen) {
    int aliveCount = 0;
    // take care of middle portion of the block
    
    for (int i = 1; i < ((int)N/p)-1; i++){
        for (int j = 1; j < N-1; j++) {
            aliveCount = 8;
            // North
            aliveCount -= (((*lastGen)[(i-1)*N + j]) & 1);
            // South
            aliveCount -= (((*lastGen)[(i+1)*N + j]) & 1);
            // East
            aliveCount -= (((*lastGen)[i*N + j+1]) & 1);
            // West
            aliveCount -= (((*lastGen)[i*N + j-1]) & 1);
            // North East
            aliveCount -= (((*lastGen)[(i-1)*N + j+1]) & 1);
            // North West
            aliveCount -= (((*lastGen)[(i-1)*N + j-1]) & 1);
            // South East
            aliveCount -= (((*lastGen)[(i+1)*N + j+1]) & 1);
            // South West
            aliveCount -= (((*lastGen)[(i+1)*N + j-1]) & 1);
            
            // Fate of cell is decided
            // cell dies of loneliness
            if (aliveCount < 3) {
                (*newGen)[i*N + j] = 1;
            }
            // cell dies of overcrowding
            else if (aliveCount > 5) {
                (*newGen)[i*N + j] = 1;
            }
            // cell is either brought back to life or stays alive
            else {
                (*newGen)[i*N + j] = 0;
            }
        }
    }


    // take care of edges

    // top row and bottom row
    for (int j = 1; j < N-1; j++) { 
        for (int i = 0; i < ((int)N/p); i += ((int)N/p)-1) {
            aliveCount = 8;
            // top row
            if (i -1 < 0){
                // North
                aliveCount -= topRow[j] & 1;
                // South
                aliveCount -= (((*lastGen)[(i+1)*N + j]) & 1);
                // North East
                aliveCount -= topRow[j+1] & 1;
                // North West
                aliveCount -= topRow[j-1] & 1;
                // South East
                aliveCount -= (((*lastGen)[(i+1)*N + j+1]) & 1);
                // South West
                aliveCount -= (((*lastGen)[(i+1)*N + j-1]) & 1);
            }
            else {
                // North
                aliveCount -= (((*lastGen)[(i-1)*N + j]) & 1);       
                // South
                aliveCount -= bottomRow[j] & 1;    
                // North East
                aliveCount -= (((*lastGen)[(i -1)*N + j+1]) & 1);
                // North West
                aliveCount -= (((*lastGen)[(i -1)*N + j-1]) & 1);
                // South East
                aliveCount -= bottomRow[j+1] & 1;
                // South West
                aliveCount -= bottomRow[j-1] & 1;
            }
            // East
            aliveCount -= (((*lastGen)[i*N + j+1]) & 1);
            // West
            aliveCount -= (((*lastGen)[i*N + j-1]) & 1);
        
            // computation
            // Fate of cell is decided
            // cell dies of loneliness
            if (aliveCount < 3) {
                (*newGen)[i*N + j] = 1;
            }
            // cell dies of overcrowding
            else if (aliveCount > 5) {
                (*newGen)[i*N + j] = 1;
            }
            // cell is either brought back to life or stays alive
            else {
                (*newGen)[i*N + j] = 0;
            }
        } 
    }  



    // left edge and right edge
    for (int i = 1; i < ((int)N/p)-1; i++) {
        for (int j = 0; j < N; j+= N-1) {
           aliveCount = 8;
           // North
           aliveCount -= (((*lastGen)[(i-1)*N + j]) & 1);
           // South
           aliveCount -= (((*lastGen)[(i+1)*N + j]) & 1);
           // East 
           aliveCount -= (((*lastGen)[i*N + ((N + j + 1) % N)]) & 1);
           // West 
           aliveCount -= (((*lastGen)[i*N + ((N + j - 1) % N)]) & 1);
           // North East
           aliveCount -= (((*lastGen)[(i-1)*N + ((N + j + 1) % N)]) & 1);
           // North West
           aliveCount -= (((*lastGen)[(i-1)*N + ((N + j - 1) % N)]) & 1);
           // South East
           aliveCount -= (((*lastGen)[(i+1)*N + ((N + j + 1) % N)]) & 1);
           // South West
           aliveCount -= (((*lastGen)[(i+1)*N + ((N + j - 1) % N)]) & 1);


           // computation
           // Fate of cell is decided
           // cell dies of loneliness
           if (aliveCount < 3) {
               (*newGen)[i*N + j] = 1;
           }
           // cell dies of overcrowding
           else if (aliveCount > 5) {
               (*newGen)[i*N + j] = 1;
           }
           // cell is either brought back to life or stays alive
           else {
               (*newGen)[i*N + j] = 0;
           }
        }
    }

    // take care of corners
    for (int i = 0; i < (int)N/p; i += ((int)N/p)-1) {
        for (int j = 0; j < N; j += N-1) {
            aliveCount = 8;
            // top row
            if (i == 0) {
                // North
                aliveCount -= (topRow[j] & 1);
                // South
                aliveCount -= (((*lastGen)[(i+1)*N + j]) & 1);
                // North East
                aliveCount -= (topRow[(N + j + 1) % N] & 1);
                // North West
                aliveCount -= (topRow[(N + j - 1) % N] & 1);
                // South East
                aliveCount -= (((*lastGen)[(i+1)*N + ((N + j + 1) % N)]) & 1);
                // South West
                aliveCount -= (((*lastGen)[(i+1)*N + ((N + j - 1) % N)]) & 1);
            }
            // bottom row
            else {
                // North
                aliveCount -= (((*lastGen)[(i-1)*N + j]) & 1);
                // South
                aliveCount -= (bottomRow[j] & 1);
                // North East
                aliveCount -= (((*lastGen)[(i-1)*N + ((N + j + 1) % N)]) & 1);
                // North West
                aliveCount -= (((*lastGen)[(i+1)*N + ((N + j - 1) % N)]) & 1);
                // South East
                aliveCount -= (bottomRow[(N + j + 1) % N] & 1);
                // South West
                aliveCount -= (bottomRow[(N + j - 1) % N] & 1);
            }
           // East
           aliveCount -= (((*lastGen)[i*N + ((N + j + 1) % N)]) & 1);
           // West
           aliveCount -= (((*lastGen)[i*N + ((N + j - 1) % N)]) & 1);
        
           // computation
           // Fate of cell is decided
           // cell dies of loneliness
           if (aliveCount < 3) {
               (*newGen)[i*N + j] = 1;
           }
           // cell dies of overcrowding
           else if (aliveCount > 5) {
               (*newGen)[i*N + j] = 1;
           }
           // cell is either brought back to life or stays alive
           else {
               (*newGen)[i*N + j] = 0;
           }
        
        }
    } 
    // copy newGen into lastGen
    for (int i = 0; i < N * ((int)N/p); i++) {
        (*lastGen)[i] = (*newGen)[i];
    }

    printLocalBlock(*lastGen, (int)N/p, N);
    return;
}
