#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define BLU "\x1B[34m"

int breakflag = 2;
int DEM = 50;
int global_matrix[50][50];
int new_matrix[50][50];
int Generation = 0;
int ALIVE = 0;
int DEAD = 1;



void displaymat(int matrix[DEM][DEM]){
  printf("\t\t\t\t---== The Current Generation is %d ==--- \n", Generation);
  for (int x = 0; x < DEM; x++){
    for (int y = 0; y < DEM; y++){
      if (matrix[y][x] == ALIVE){
	printf("%s■ ", BLU);
      }else{
	printf("□ ");
      }
    }
    printf("\n");
  }
  Generation++;
  printf("\n\n\n");
}

int check(int matrix[DEM][DEM], int xpos, int ypos){
  int count = 0;
  for (int xi = -1; xi <=1 ; xi++){
     for (int yi = -1; yi <= 1; yi++){
       int nxpos = xi + xpos;
       if (nxpos < 0) nxpos = DEM-1;
       if (nxpos == DEM) nxpos = 0;

       int nypos = yi + ypos;
       if (nypos < 0) nypos = DEM-1;
       if (nxpos == DEM) nypos = 0;
	
       if (matrix[nxpos][nypos] == ALIVE){
	   count++;
	 }
     }
  }
  if (count ==3 || (count == 4) && (matrix[xpos][ypos] == ALIVE)){
    return (ALIVE); 
  }
  return(DEAD);
}

void swap (int matrix[DEM][DEM], int nmatrix[DEM][DEM]){
  for (int x = 0; x < DEM; x++){
    for (int y = 0; y < DEM; y++){
      matrix[x][y] = nmatrix[x][y];
    }
  }
}



void logic (int matrix[DEM][DEM], int nmatrix[DEM][DEM]){
  for (int x = 0; x < DEM; x++){
    for (int y = 0; y < DEM; y++){
      nmatrix[x][y] = check(matrix, x, y);
    }
  }
  swap(matrix, nmatrix);
}



void update (int sig){
  displaymat(global_matrix);
  logic(global_matrix, new_matrix);
  breakflag--;
  alarm(1);
}

void init_matrix(int matrix[DEM][DEM]){
  for (int x = 0; x < DEM; x++){
    for (int y = 0; y < DEM; y++){
      matrix[y][x] = rand() % 4;
      if (matrix[y][x] > 1){
	matrix[y][x] = DEAD;
      }
    }
    
  }
}
void make_ship(int matrix[DEM][DEM]){
  for (int x = 0; x < DEM; x++){
    for (int y = 0; y < DEM; y++){
      matrix[y][x] = DEAD;
    }
  }
  matrix[25][25] = ALIVE;
  matrix[26][24] = ALIVE;
  matrix[25][26] = ALIVE;
  matrix[25][27] = ALIVE;
  matrix[26][27] = ALIVE;
  matrix[27][27] = ALIVE;
  matrix[28][27] = ALIVE;
  matrix[29][26] = ALIVE;
  matrix[29][24] = ALIVE;
  

  
  
  
  
}
int main(int argc, char** argv) {
  signal(SIGALRM, update);
  if (argc == 2){
    init_matrix(global_matrix);
    breakflag = atoi(argv[1]);
  }else{
    breakflag = atoi(argv[1]);
    make_ship(global_matrix);
  }
  
  alarm(1);
  while(breakflag) {sleep(1);}
  
}
