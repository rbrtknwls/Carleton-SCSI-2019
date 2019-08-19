#include "mpi.h"
#include <stdio.h>
#include <unistd.h>

#define DEM 50          /* the size of vector */
#define FROM_MASTER 1     /* message tag */
#define FROM_WORKER 2     /* message tag */
#define MASTER 0          /* master has rank of 0 */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

int main(int argc, char *argv[] ) {

    int nprocs,         /* number of processes in MPI_COMM_WORLD */
        rank,           /* the rank of each process */
        i, j,
        gen;

    int averow,         /* average rows to be sent to each worker */
        extra,          /* extra rows to be sent to some workers */
        offset,
        dest,           /* rank of the destination process */
        numworkers,     /* storing number of workers */
        mtype,          /* message type */
        source,         /* rank of the source process */
        rows;           /* rows to be sent to each process */

    int matrix[DEM][DEM];   /* matrix for multiplication */

    MPI_Status status;          /* status for receiving */

    /* Initialize MPI execution environment */
    MPI_Init( &argc,&argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs);

    if (rank == 0){
      for(i=0; i < DEM; i++) {
          // Change here to use random integer
          for(j = 0; j < DEM; j++) {
              // Change here to use random integer
              matrix[i][j] = -1;
          }
      }
      matrix[0][2] = 1;
    }
    for (int i = 0; i < 200; i++){
      /************************* Master ************************/
      if (rank == 0) {
          printf("\n\n\n\t\t\t------------=== Generation: %d ===------------\n", gen);
      /* Initialize Matrix and Vector */

           /* number of workers in the communicator MPI_COMM_WORLD */
          numworkers = nprocs - 1;

          /* decomposing the rows of matrix for each worker */
          averow = DEM/numworkers;
          extra = DEM%numworkers;
          offset = 0;
          mtype = FROM_MASTER;

          /* sending each task to each worker, some workers will get more others
           * if number of rows is not divisible by number of processes
           */

           // TO DO:
          for(dest = 1; dest <= numworkers; dest++) {
              rows = (dest <= extra) ? averow + 1 : averow;

              printf("sending %d the %d", dest, rows);
              if (dest == 1 && dest == numworkers){
                MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&matrix[offset][0], rows * DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Top Extra
                MPI_Send(&matrix[DEM-1][0],DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Bottom Extra
                MPI_Send(&matrix[0],DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);

              }else if (dest == 1){
                MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);

                // LOOK HERE
                MPI_Send(&matrix[offset][0], rows * DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Top row
                MPI_Send(&matrix[DEM-1], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Bottom Rebound
                MPI_Send(&matrix[offset+1], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
              }else if (dest == numworkers){
                MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&matrix[offset][0], rows * DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&matrix[offset-1], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Bottom Rebound
                MPI_Send(&matrix[0], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
              }
              offset += rows;
          }

          mtype = FROM_WORKER;
          for (i = 1; i <= numworkers; i++) {
               source = i;
               MPI_Recv(&offset, 1, MPI_INT, source,mtype, MPI_COMM_WORLD, &status);
               MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
               MPI_Recv(&matrix[offset][0], rows * DEM, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
          }

          /* Printing results */
          for (int x = 0; x < DEM; x++) {
              for (int y = 0; y < DEM; y++){
                // Pros 1
                if (matrix[x][y] == 1){
                  printf("%s■ ", KRED);
                }
                if (matrix[x][y] == -1){
                  printf("%s□ ", KRED);
                }
                // Pros 2
                if (matrix[x][y] == 2){
                  printf("%s■ ", KBLU);
                }
                if (matrix[x][y] == -2){
                  printf("%s□ ", KBLU);
                }
                // Pros 3
                if (matrix[x][y] == 3){
                  printf("%s■ ", KYEL);
                }
                if (matrix[x][y] == -3){
                  printf("%s□ ", KYEL);
                }
                // Pros 4
                if (matrix[x][y] == 4){
                  printf("%s■ ", KGRN);
                }
                if (matrix[x][y] == -4){
                  printf("%s□ ", KGRN);
                }
                // Pros 4
                if (matrix[x][y] == 5){
                  printf("%s■ ", KWHT);
                }
                if (matrix[x][y] == -5){
                  printf("%s□ ", KWHT);
                }
              }
              printf("\n%s", KNRM);
          }
          gen++;


      }

      /* the workers receive task from master, and will do the computations */
      if (rank > 0) {
          mtype = FROM_MASTER;
          int topmatrix[DEM];
          int bottommatrix[DEM];
          /* Receive the task from master */
          // TO DO:

          MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
          MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
          MPI_Recv(&matrix, rows*DEM, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

          MPI_Recv(&topmatrix, DEM, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
          MPI_Recv(&bottommatrix, DEM, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
          // end TO DO

          int nmatrix[rows][DEM];
          /* Each worker works on their computation */
          for(int x = 0; x < DEM; x++) {
              for(int y = 0; y < rows; y++) {
                  int count = 0;
                  for (int xi = -1; xi <= 1; xi++){
                    for (int yi = -1; yi <= 1; yi++){
                      int xpos = x + xi;
                      int ypos = y + yi;

                      if (xpos < 0) xpos = DEM-1;
                      if (xpos == DEM) xpos = 0;

                      if (ypos < 0){
                        if (topmatrix[xpos] > 0){
                          count++;
                        }
                      }
                      if (ypos == rows){
                         if (bottommatrix[xpos] > 0){
                           count++;
                         }
                      }

                      if ((matrix[ypos][xpos] > 0) && (ypos >= 0 && ypos < rows)){
                        count++;
                      }

                    }
                  }
                  if (count > 0){
                    nmatrix[y][x] = rank;
                  }else{
                    nmatrix[y][x] = rank*-1;
                  }
              }
          }
          /* send the result back to the master */
          mtype = FROM_WORKER;
          // TO DO:
          MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          MPI_Send(&nmatrix, rows*DEM, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          //end TO DO
      }
    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
    }
    /* Terminate the MPI execution environment */
    MPI_Finalize();
    return 0;
}
