#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int testPrimeNumber(int number, int qtdTests);

int main(int argc, char const *argv[]){

	int qtdTests = 10000000; //The amount of numbers to be finded
	int testNumber = 3; //The amount of numbers witch has already been tested
	clock_t begin, end; //To see the processing time
	int thread_count; //The number of threads the program will use

	thread_count = strtol(argv[1],NULL,10);

	begin = clock();

	printf("1\n2\n");
	//int test = qtdTests/thread_count;
//#pragma omp parallel num_threads(thread_count)
//# pragma omp parallel for num_threads(thread_count)
#pragma omp parallel for num_threads(thread_count) schedule(static,thread_count) 
	for(testNumber = 3;testNumber < qtdTests;testNumber++){

		if(testPrimeNumber(testNumber,qtdTests) == 1){
				printf("%d\n", testNumber);
		}

	}

	end = clock();

	printf("Total Time: %ld\n\n",end-begin);

	return 0;
}

int testPrimeNumber(int number, int qtdTests){

	int i;

	for(i=3;i<=sqrt(number);i++){

		if (number%i == 0){
			return 0;
		}
	}

	return 1;
}
