/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *	
 *	Example code
 *	Send receive test:
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *   	For timing use of C gettimeofday() is recommended.
 * */


#include <stdio.h>
#include <mpi.h> 
#include <assert.h>
#include <sys/time.h>

int main(int argc,char *argv[])
{

   int rank,p;
   struct timeval t1,t2;

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   printf("my rank=%d\n",rank);
   printf("Rank=%d: number of processes =%d\n",rank,p);

   assert(p>=2);


   if(rank==1) {
		int x = 10;
		int dest = 0;
		gettimeofday(&t1,NULL);
        /*
         * &x = address of local byte buffer - data that you want to send
         * count = number of elements we want to send
         * data type = MPI_INT/MPI_CHAR
         * destination rank = where to send the data
         * tag = say multiple messages are expected from the same ID, to handpick from all the message
         * Handle for globe of communicating world
         */
		MPI_Send(&x,1,MPI_INT,dest,0,MPI_COMM_WORLD);
		gettimeofday(&t2,NULL);
		int tSend = (t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)/1000;

		printf("Rank=%d: sent message %d to rank %d; Send time %d millisec\n", rank,x,dest,tSend);
   } else 
   if (rank==0) {
   		int y=0;
        // MPI_STATUS is like storing the metadata like headers of HTML
		MPI_Status status;
		gettimeofday(&t1,NULL);
        /*
         * Location where data should be stored
         * units expected receive, in this case 1
         * size of the unit, in our case MPI_INT = 4 bytes
         * sending source = in our case receive from anyone -> MPI_ANY_SOURCE
         * TAG = ANY_TAG
         * COMM_WORLD
         * store meta data in status
         */
   		MPI_Recv(&y,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		gettimeofday(&t2,NULL);
		int tRecv = (t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)/1000;
		printf("Rank=%d: received message %d from rank %d; Recv time %d millisec\n",rank,y,status.MPI_SOURCE,tRecv);
   }

   MPI_Finalize();
}
