/*
 * @author Adiessa Bell and Nathanael Pelling
 * @date 10/12/2021
 * @info course COP4634
*/

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdio>
#include "timer.hpp"

using namespace std;

mutex mx;
int currentCollatz = 2;
int numOfCollatz = 0;
vector<int> computedTimes;


int computeCollatz(int num){
	int numOfIterations = 0;
	int valueSoFar = num;

	if(num != 1){
		while(valueSoFar != 1){
			//The number is even if its divided by 2, and the result is zero.
			if(valueSoFar % 2 == 0){
				valueSoFar /= 2;
				numOfIterations++;
			}

			//The number is negative.
			else{
				valueSoFar = (valueSoFar * 3) + 1;
				numOfIterations++;
			}
		}
	}

	else{
		valueSoFar = (valueSoFar * 3) + 1;

		while(valueSoFar != 1){
			//The number is even if ites divided by 2, and the result is zero.
			if(valueSoFar % 2 == 0){
				valueSoFar /= 2;
				numOfIterations++;
			}

			//The number is negative.
			else{
				valueSoFar = (valueSoFar * 3) + 1;
				numOfIterations++;
			}
		}
	}

	return numOfIterations;
}

void* threadInstructions(){
	while(currentCollatz <= numOfCollatz){
		computedTimes.at(currentCollatz - 1) = computeCollatz(currentCollatz);

		//Lock variable right here I guess.
		if(lock == true){
			mx.lock();
		}

		currentCollatz++;

		//Unlock variable right here I guess.
		if(lock == true){
			mx.unlock();
		}
	}

	pthread_exit(NULL);
}


int main(int ARG_COUNT, char* argVect[]){
	int numOfThreads = 0;
	bool lock = true;
	bool outputRedirect = false;
	bool outputRedirectAppend = false;
	bool errorRedirect = false;
	string fileName = "";
	Timer time;
	pthread_t* threads;
	
	cout << "Hello" << endl;

	time.start();
	computedTimes.at(0) = computeCollatz(1);
	/*

	//If their are six arguments passed to the program we will do the file redirect and check for the -nolock command.
	if(ARG_COUNT == 6){
		if(argVect[1].isdigit()){
			if((argVect[1] < 2) || (argVect[1] > 10000)){
				cout << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

				exit(-1);
			}

			else{
				numOfCollatz = argVect[1];

				for(int i = 0; i < numOfCollatz; i++){
					computedTimes.push_back(0);
				}
			}
		}

		else{
			cout << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}

		if(argVect[2].isdigit()){
			if(argVect[2] > numOfCollatz){
				cout << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = argVect[2];
				threads = new pthread_t[numOfThreads];
			}
		}

		else{
			cout << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}

		if(strcmp(argVect[3], "-nolock") == 0){
			lock = false;
		}

		else{
			cout << "ERROR: Expected \"-nolock\" command." << endl;

			exit(-1);
		}

		if(strcmp(argVect[4], ">") == 0){
			outputRedirect = true;
		}

		else if(strcmp(argVect[4], "2>") == 0){
			errorRedirect = true;
		}

		else if(strcmp(argVect[4], ">>") == 0){
			outputRedirectAppend = true;
		}

		else if(strcmp(argVect[4], "2>>") == 0){
			errorRedirectAppend = true;
		}

		else{
			cout << "ERROR: Expected the \">\", \"2>\", \">>\" or \"2>>\" redirect symbols." << endl;

			exit(-1);
		}

		if(argVect[5].isalnum()){
			fileName = argVect[5];

			if(outputRedirect == true){
				freopen((char*)fileName, "w", stdout);
			}

			else if(errorRedirect == true){
				freopen((char*)fileName, "w", stderr);
			}

			else if(outputRedirectAppend == true){
				freopen((char*)fileName, "a", stdout);
			}

			else if(errorRedirectAppend == true){
				freopen((char*)fileName, "a", stderr);
			}
		}

		else{
			cout << "ERROR: Expected a file name here." << endl;

			exit(-1);	
		}
	}

	else if(ARG_COUNT == 4){
		if(argVect[1].isdigit()){
			if((argVect[1] < 2) || (argVect[1] > 10000)){
				cout << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

				exit(-1);
			}

			else{
				numOfCollatz = argVect[1];

				for(int i = 0; i < numOfCollatz; i++){
					computedTimes.push_back(0);
				}
			}
		}

		else{
			cout << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}

		if(argVect[2].isdigit()){
			if(argVect[2] > numOfCallatz){
				cout << "ERROR: the number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = argVect[2];
				threads = new pthread_t[numOfThreads];
			}
		}

		else{
			cout << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}

		if(strcmp(argVect[3], "-nolock") == 0){
			lock = false;
		}

		else{
			cout << "ERROR: Expected the \"-nolock\" command." << endl;

			exit(-1);
		}
	}

	else if(ARG_COUNT == 3){
		if(argVect[1].isdigit()){
			if((argVect[1] < 2) || (argVect[1] > 10000)){
				cout << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

				exit(-1);
			}

			else{
				numOfCollatz = argVect[1];

				for(int i = 0; i < numOfCollatz; i++){
					computedTimes.push_back(0);
				}
			}
		}

		else{
			cout << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}
	
		if(argVect[2].isdigit()){
			if(argVect[2] > numOfCollatz){
				cout << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = argVect[2];
				threads = new pthread_t[numOfThreads];
			}
		}

		else{
			cout << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}
	}

	int returnValue = 0;

	for(int i = 0; i < numOfThreads; i++){
		returnValue = pthread_create(&threads[i], NULL, threadInstructions, NULL);

		if(returnValue){
			perror("ERROR: Unable to create thread.");
		}
	}

	delete threads;

	time.stop();

	pthread_exit(NULL);
	*/
}
