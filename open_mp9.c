/*
 ============================================================================
 Name        : open_mp9.c
 Author      : Hugo Cafferty
 Version     : 9
 Description : An OpenMP program of Conway's Game of Life
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double floor(double arg);
int boardSet(int display);
int nextTt(int lower,int upper);

//Stores the alive and dead cells and where they are
int board[50][50];
//Stores the changes being made to board
int nextBoard[50][50];
//Stores the number of live cells surrounding a cell
int areaBoard[50][50];


int main (int argc, char *argv[]) {
	clock_t start = clock();

	//Sets all values in areaBoard to 0
	for(int i=0; i<50; i++){
		for(int k=0; k<50; k++){
			areaBoard[i][k]=0;
		}
	}
	//*******

	//Sets all values in nextBoard to 0
	for(int i=0; i<50; i++){
		for(int k=0; k<50; k++){
			nextBoard[i][k]=0;
		}
	}
	//*******

	//Used to store the thread ID
	int tid;

	//Populate the array with dead and live cells
	boardSet(1);

	//The number of threads to be used
	int tSet=1;

	//An int representing the number of rows left over after dividing the number of rows
    int remainder = 50%tSet;

    //Set the number of threads
	omp_set_num_threads(tSet);

	//Loop through i generations
	for(int i=0; i<300; i++){
		//create threads
		#pragma omp parallel private(tid)
		{
		   //store the current thread ID
		   tid = omp_get_thread_num();

		   //If not on the last thread and if there is no remainder then calculate the next iteration for the next 50/tid rows
		   if(tid!=tSet-1 && remainder!=0){
			   nextTt(((int)floor(50/tSet))*tid,((int)floor(50/tSet))+(((int)floor(50/tSet))*tid));
		   }
		   //Else calculate the next iteration for the final section of rows + the remainding number of rows
		   else{
			   nextTt(((int)floor(50/tSet))*tid,((int)floor(50/tSet))+(((int)floor(50/tSet))*tid)+remainder);
		   }

		}

		//overwite the board with nextBoard
		for(int i=0; i<50; i++){
			for(int k=0; k<50; k++){
				board[i][k]=nextBoard[i][k];
				nextBoard[i][k]=0;
			}
		}
		//reset areaBoard to 0s
		for(int i=0; i<50; i++){
			for(int k=0; k<50; k++){
				areaBoard[i][k]=0;
			}
		}
	}


	//print the board
	printf("\n***********************************************\n");
	for(int i=0; i<50; i++){
		for(int k=0; k<50; k++){
			printf(",%d",board[i][k]);
		}
		printf("\n");
	}

	//end the clock and print the results
	clock_t end = clock();
	double total =  (double)(end-start) / CLOCKS_PER_SEC;
	printf("Program took %fs \n",total);
	getchar();
	return 0;
}





//calculate the next board by scanning around a board index adding 1 to the same index in areaBoard.
//Then using the results from areaBoard determin the value for the same index on nextBoard using the Game Of Life rules
int nextTt(int lower,int upper){
	for(int i=lower; i<upper; i++){
		for(int k=0; k<50; k++){
			//check top
			if(board[i-1][k]==1 && i!=0){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check top right
			if(board[i-1][k+1]==1 && i!=0 && k!=49){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check right
			if(board[i][k+1]==1 && k!=49){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check low right
			if(board[i+1][k+1]==1 && i!=49 && k!=49){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check low
			if(board[i+1][k]==1 && i!=49){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check low left
			if(board[i+1][k-1]==1 && i!=49 && k!=0){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check left
			if(board[i][k-1]==1 && k!=0){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			//check top left
			if(board[i-1][k-1]==1 && i!=0 && k!=0){
				areaBoard[i][k]=areaBoard[i][k]+1;
			}
			if(areaBoard[i][k]<2){nextBoard[i][k]=0;}
			if((areaBoard[i][k]==2 && board[i][k]==1) || (areaBoard[i][k]==3 && board[i][k]==1)){nextBoard[i][k]=1;}
			if(areaBoard[i][k]>3){nextBoard[i][k]=0;}
			if(areaBoard[i][k]==3){nextBoard[i][k]=1;}
		}
	}

	return 0;
}

int boardSet(int display){
	//0=dead cell, 1=live cell
	//srand (time(NULL) * getpid());
	int random = rand() % 6;

	//if the random number is 1 the cell will = 1 else it will be 0
	for(int i=0; i<50; i++){
		for(int k=0; k<50; k++){
				random = rand() % 3;
				//printf("%d\n",random);
				if(random==1){
					board[i][k]=1;
				}
				else{
					board[i][k]=0;
				}
				if(display == 1){
					printf(",%d",board[i][k]);
				}
		}
		printf("\n");
	}
	return 0;
}
