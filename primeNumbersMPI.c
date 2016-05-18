/*
Check list
	**Manager roles
	 A1 - Divide in equal parts the amount of numbers in wich each worker will test
	 A2 - Send for each worker the amount of numbers it will test
	 A3 - Send the N numbers for the workers
	 A4 - Recive the result of the test of prime number
	 A5 - Print the number if it is a prime number

    **Workers roles
	 B1 - Recive the amount of numbers to be tested
	 B2 - Define the amount of loops necessary
	 B3 - Recive the number to be tested
	 B4 - Test if this number is a prime number
	 B5 - Send to the manager the result of the test of prime number
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define AMOUNTTOBETESTED 10000 //The range of numbers to be tested

int main(int argc, char** argv){
	
	int my_rank; //The rank each process will have during the execution
	int p; //The amount of processes running into the program
	int source; //The source of the information sent. It'll represent the rank of it.
	int dest; //The destiny of the information sent. It'll represent the rank of it.
	int tag; //The tag of the information. It needs to be set in the source and in the dest as the same to send a message.
	int current_tested_number; //The number in witch the test is been applied.
	int is_prime; //An boolean value that represents if the number is prime(1) or not(0).
	int amountOfTests; //The amount of tests each worker will do
	MPI_Status status;

	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	 if(my_rank == 0){
	 	//A1
	 	if(AMOUNTTOBETESTED%(p-1) == 0){
	 		amountOfTests = AMOUNTTOBETESTED;
	 	}else{
	 		int intPartOfTheNumber = (int)(AMOUNTTOBETESTED/(p-1));
	 		amountOfTests = intPartOfTheNumber * (p-1);
	 	}

	 	amountOfTests /= (p-1);

	 	//A2
	 	for(dest=1;dest<=p;dest++){
	 		MPI_Send(&amountOfTests,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
	 	}

	 	for(current_tested_number=1;current_tested_number<=AMOUNTTOBETESTED;){
	 		for(dest=1;dest<=p;dest++){
	 			//A3
	 			current_tested_number++;
	 			MPI_Send(&current_tested_number,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
	 		}

	 		for(source=1;source<=p;source++){
	 			//A4
	 			MPI_Recv(&is_prime,1,MPI_INT,source,tag,MPI_COMM_WORLD,status);

	 			//A5
	 			if(is_prime == 1){
	 				printf("%d is a prime number!\n", current_tested_number-(p-1)+source);
	 			}
	 		}
	 	}

	 }else{
	 	//B1
	 	MPI_Recv(&amountOfTests,1,MPI_INT,0,tag,MPI_COMM_WORLD,status);

	 	//B2
	 	int i;
	 	for(i=0;i<amountOfTests;i++){
	 		//B3
	 		MPI_Recv(&current_tested_number,1,MPI_INT,0,tag,MPI_COMM_WORLD,status);

	 		//B4
	 		is_prime = isPrimeNumber(current_tested_number);

	 		//B5
	 		MPI_Send(&is_prime,1,MPI_INT,0,tag,MPI_COMM_WORLD);
	 	}
	 }

	MPI_Finalize();

	return 0;
}

/* 
 0 means it's not a prime number
 1 means it's a prime number
*/
int isPrimeNumber(int numberToBeTested){
	
	//If the number exceeds the real amount to be tested it just return it's not prime, even though it is!!!
	//It does not mean 1,2 aren't prime number, it just mean they do not need to be tested!!!
	//If the number is even it's not a prime number!!!	
	if(numberToBeTested > AMOUNTTOBETESTED || numberToBeTested < 3 || numberToBeTested%2 == 0){
		return 0;
	}

	int i;
	for(i=3;i<=numberToBeTested/2;i++){
		/*
			If there is any number in wich the rest of the division of the current tested number by 
			that number is equal to 0 it means that the current tested number is not a prime number!!!
		*/
		if(numberToBeTested%i == 0){
			return 0;
		}
	}

	return 1;

}