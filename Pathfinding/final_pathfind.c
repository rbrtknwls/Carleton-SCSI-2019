#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define DEM 20            /* the size of vector */
#define FROM_MASTER 1     /* message tag */
#define FROM_WORKER 2     /* message tag */
#define MASTER 0          /* master has rank of 0 */
#define UNVISITED -5
#define BLOCKED -2
#define START 0
#define END -30

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KRND  "\x1B[30m"


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

    int matrix[DEM][DEM];
    int END_POS = 8;
    int color_matrix[DEM][DEM];   /* matrix for multiplication */

    MPI_Status status;          /* status for receiving */
    int GENERATION = 15;
    int Stawp = 0;
    /* Initialize MPI execution environment */
    MPI_Init( &argc,&argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs);
    double start = MPI_Wtime();
    if (rank == 0){
      for (int x = 0; x < DEM; x++){
        for (int y = 0; y < DEM; y++){
          matrix[y][x] = UNVISITED;
          if(x == 0 || y == 0 || x == DEM-1 || y == DEM-1){
              matrix[y][x] = BLOCKED;
          }
        }
      }
      matrix[15][2] = START;
      matrix[END_POS][END_POS] = END;
    }
    for (int i = 0; i < GENERATION; i++){
      /************************* Master ************************/
      if (rank == 0) {
          //printf("\n\n\n\t\t\t------------=== Generation: %d ===------------\n", gen);
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
                // Top Extra
                MPI_Send(&matrix[DEM-1], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Bottom Extra
                MPI_Send(&matrix[rows], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
              }else if (dest == numworkers){
                MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&matrix[offset][0], rows * DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Top Extra
                MPI_Send(&matrix[offset-1], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Bottom Extra
                MPI_Send(&matrix[0], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
              }else{
                MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&matrix[offset][0], rows * DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Top Extra
                MPI_Send(&matrix[offset-1], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                // Bottom Extra
                MPI_Send(&matrix[offset+rows], DEM, MPI_INT, dest, mtype, MPI_COMM_WORLD);
              }
              offset += rows;
          }

          mtype = FROM_WORKER;
          for (int x = 1; x <= numworkers; x++) {
               source = x;
               MPI_Recv(&offset, 1, MPI_INT, source,mtype, MPI_COMM_WORLD, &status);
               MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
               MPI_Recv(&matrix[offset][0], rows * DEM, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
               MPI_Recv(&color_matrix[offset][0], rows * DEM, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
               MPI_Recv(&Stawp, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
          }
          /* Printing results */
          if (Stawp == 1){
            i+= 100000000;
          }
          printf("\t\t\t------------=== Generation: %d ===------------\n", gen);
          for (int x = 0; x < DEM; x++) {
              if (abs(color_matrix[x][0]) == 1){
                printf("%s", KRED);
              }else if (abs(color_matrix[x][0]) == 2){
                printf("%s", KBLU);
              }else if (abs(color_matrix[x][0]) == 3){
                printf("%s", KYEL);
              }else if (abs(color_matrix[x][0]) == 4){
                printf("%s", KGRN);
              }else if (abs(color_matrix[x][0]) == 5){
                printf("%s", KWHT);
              }else if (abs(color_matrix[x][0]) == 6){
                printf("%s", KCYN);
              }else if (abs(color_matrix[x][0]) == 7){
                printf("%s", KMAG);
              }else if (abs(color_matrix[x][0]) == 8){
                printf("%s", KRND);
              }
              for (int y = 0; y < DEM; y++){
                if (matrix[x][y] > 0){
                  printf("%d ", matrix[x][y]);
                }else if (matrix[x][y] == UNVISITED){
                  printf("□ ");
                }else if (matrix[x][y] == START){
                  printf("S ");
                }else if (matrix[x][y] == END){
                  printf("E ");
                }else if (matrix[x][y] == BLOCKED){
                  printf("▲ ");
                }
              }
              printf("\n%s", KNRM);
          }
          gen++;
          printf("\n\n\n");



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
          int color_matrix[rows][DEM];
          int Stawp = 0;
          /* Each worker works on their computation */

          for(int x = 0; x < DEM; x++) {
              for(int y = 0; y < rows; y++) {
                if (matrix[y][x] == UNVISITED){
                  int check = 0;
                  for (int xi = -1; xi <=1 ; xi++){
                    for (int yi = -1; yi <= 1; yi++){
                      if (abs(xi) != abs(yi)){
                        int nxpos = xi + x;
                        if (nxpos <= 0) nxpos = 0;
                        if (nxpos == DEM) nxpos = DEM-1;

                        int nypos = yi + y;
                        if (nypos < 0){
                          if (topmatrix[nxpos] >= gen){

                            nmatrix[y][x] = i+1;
                            check = 1;
                          }
                        }else if (nypos >= rows){
                           if (bottommatrix[nxpos] >= gen){
                             nmatrix[y][x] = i+1;
                             check = 1;
                           }
                        }else if ((matrix[nypos][nxpos] >= gen)){
                          nmatrix[y][x] = i+1;
                          check = 1;
                        }
                      }
                    }
                  }

                  if (check == 0){

                    nmatrix[y][x] = matrix[y][x];
                  }
                }else if (matrix[y][x] == END){
                  nmatrix[y][x] = matrix[y][x];
                  for (int xi = -1; xi <=1 ; xi++){
                    for (int yi = -1; yi <= 1; yi++){
                      if (abs(xi) != abs(yi)){
                        int nxpos = xi + x;
                        if (nxpos <= 0) nxpos = 0;
                        if (nxpos == DEM) nxpos = DEM-1;
                        int nypos = yi + y;
                        if (matrix[nypos][nxpos] >= gen){
                          nmatrix[y][x] = i+1;
                          Stawp = 1;
                        }
                      }
                    }
                  }
                }else{
                  nmatrix[y][x] = matrix[y][x];

                }
                color_matrix[y][x] = rank;
              }
          }
          /* send the result back to the master */
          mtype = FROM_WORKER;
          // TO DO:
          MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          MPI_Send(&nmatrix, rows*DEM, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          MPI_Send(&color_matrix, rows*DEM, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          MPI_Send(&Stawp, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
          //end TO DO
      }
    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
    }
    //for (int i = 0; i < END_POS; i++){

    //}
    /* Terminate the MPI execution environment */
    if (rank == 0){
      int vals_to_loop = matrix[END_POS][END_POS];
      int Curr_X, Curr_Y;
      Curr_X = END_POS;
      Curr_Y = END_POS;
      for (int size = vals_to_loop; size > 0; size--){
        int thing = 0;
        for (int xi = -1; xi <=1 ; xi++){
          for (int yi = -1; yi <= 1; yi++){
            if (abs(xi) != abs(yi)){
              int nxpos = xi + Curr_X;
              int nypos = yi + Curr_Y;

              if (matrix[nypos][nxpos] > 0 && thing != 1 && matrix[nypos][nxpos] < size){
                matrix[Curr_Y][Curr_X] = -50;
                Curr_X = nxpos;
                Curr_Y = nypos;
                thing = 1;
              }
            }

          }
        }

      }
      for (int x = 0; x < DEM; x++) {
          if (abs(color_matrix[x][0]) == 1){
            printf("%s", KRED);
          }else if (abs(color_matrix[x][0]) == 2){
            printf("%s", KBLU);
          }else if (abs(color_matrix[x][0]) == 3){
            printf("%s", KYEL);
          }else if (abs(color_matrix[x][0]) == 4){
            printf("%s", KGRN);
          }else if (abs(color_matrix[x][0]) == 5){
            printf("%s", KWHT);
          }else if (abs(color_matrix[x][0]) == 6){
            printf("%s", KCYN);
          }else if (abs(color_matrix[x][0]) == 7){
            printf("%s", KMAG);
          }else if (abs(color_matrix[x][0]) == 8){
            printf("%s", KRND);
          }
          for (int y = 0; y < DEM; y++){
            if (x == END_POS && y == END_POS){
              printf("E ");
            }
            else if (matrix[x][y] == -50){
              printf("■ ");
            }else if (matrix[x][y] == START){
              printf("S ");
            }else if (matrix[x][y] == BLOCKED){
              printf("▲ ");
            }else{
              printf("□ ");
            }
          }
          printf("\n%s", KNRM);
      }
      printf("%d", vals_to_loop);
      double end = MPI_Wtime();
      printf("it took %f seconds \n", end - start);
    }
    MPI_Finalize();
    return 0;
}
