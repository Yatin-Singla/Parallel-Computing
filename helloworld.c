/*  Cpt S 411, Introduction to Parallel Computing
 *  School of Electrical Engineering and Computer Science
 *    
 *  Example code
 *  	Hello world MPI
 *          
 */


#include <stdio.h>
#include <mpi.h> 

int main(int argc,char *argv[])
{

    // rank = what is my rank/processID
    // p = total number of processes
   int rank,p;


    /*Initialization*/
    // MPI_Init creates all the IDs and assigns an ID for every process
   MPI_Init(&argc,&argv);
   //assign my rank
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   //what is the total number of processes and store it in p
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   printf("my rank=%d\n",rank);
   printf("Rank=%d: number of processes =%d\n",rank,p);

   //while(1) {
   //}

    // end the parallel program
   MPI_Finalize();
}
