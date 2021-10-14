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

//using namespace std;
using std::cout;
using std::cin;
using std::cerr;
using std::mutex;
using std::vector;
using std::string;
using std::endl;
using std::thread;

mutex mx;
int currentCollatz = 2;
int numOfCollatz = 0;
bool lock = true;
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

void threadInstructions(){
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
	//pthread_exit(NULL);
}

int main(int ARG_COUNT, char* argVect[]){
	int numOfThreads = 0;
	bool outputRedirect = false;
	bool outputRedirectAppend = false;
	bool errorRedirect = false;
	bool errorRedirectAppend = false;
	string fileName = "";
	string 
	Timer time;
	//pthread_t* threads;
	//thread* threads;
	
	cout << "Hello" << endl;

	time.start();
	//computedTimes.at(0) = computeCollatz(1);

	//If their are six arguments passed to the program we will do the file redirect and check for the -nolock command.
	if(ARG_COUNT == 6){
		if(isdigit((int)(argVect[1]))){
			if((argVect[1] < 2) || (argVect[1] > 10000)){
				cerr << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

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
			cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}

		if(isdigit((int)argVect[2])){
			if(argVect[2] > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = argVect[2];
				threads = new pthread_t[numOfThreads];
			}
		}

		else{
			cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}

		if(strcmp(argVect[3], "-nolock") == 0){
			lock = false;
		}

		else{
			cerr << "ERROR: Expected \"-nolock\" command." << endl;

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
			cerr << "ERROR: Expected the \">\", \"2>\", \">>\" or \"2>>\" redirect symbols." << endl;

			exit(-1);
		}

		if(isalnum(argVect[5])){
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
			cerr << "ERROR: Expected a file name here." << endl;

			exit(-1);	
		}
	}

	/*
	else if(ARG_COUNT == 4){
		if(argVect[1].isdigit()){
			if((argVect[1] < 2) || (argVect[1] > 10000)){
				cerr << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

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
			cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}

		if(argVect[2].isdigit()){
			if(argVect[2] > numOfCallatz){
				cerr << "ERROR: the number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = argVect[2];
				threads = new pthread_t[numOfThreads];
			}
		}

		else{
			cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}

		if(strcmp(argVect[3], "-nolock") == 0){
			lock = false;
		}

		else{
			cerr << "ERROR: Expected the \"-nolock\" command." << endl;

			exit(-1);
		}
	}

	else if(ARG_COUNT == 3){
		if(argVect[1].isdigit()){
			if((argVect[1] < 2) || (argVect[1] > 10000)){
				cerr << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

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
			cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}
	
		if(argVect[2].isdigit()){
			if(argVect[2] > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = argVect[2];
				threads = new pthread_t[numOfThreads];
			}
		}

		else{
			cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}
	}

	//int returnValue = 0;
	thread threads[numOfThreads];

	for(int i = 0; i < numOfThreads; i++){
		//returnValue = pthread_create(&threads[i], NULL, threadInstructions, NULL);
		threads[i] = thread(threadInstructions);

		//if(returnValue){
			//cerr << "ERROR: Unable to create thread." << endl;
		//}
	}

	computedTimes.at(0) = computeCollatz(1);

	for(int i = 0; i < numOfThreads; i++){
		threads[i].join();
	}

	delete threads;

	time.stop();

	for(long unsigned int i = 0; i < computedTimes.size(); i++){
		cout << i + 1 << "," << computedTimes.at(i) << endl;
	}

	cerr << numOfCollatz << ", " << numOfThreads << ", " << time.getElapsedTime() << endl;
	//pthread_exit(NULL);
	*/
	
	return 0;
}
