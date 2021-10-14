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
#include <cstring>
#include "timer.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::mutex;
using std::vector;
using std::string;
using std::endl;
using std::thread;
using std::stoi;

mutex mx;
int currentCollatz = 2;
int numOfCollatz = 0;
char* array;
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

		//Lock variable right here so other threads cant change it at the same time causing undefined beavior.
		if(lock == true){
			mx.lock();
		}

		currentCollatz++;

		//Unlock variable right so that other threads can update the currentCollatz variable.
		if(lock == true){
			mx.unlock();
		}
	}
}

char* cStringToCxx(string word){
	array = new char[word.size()];

	for(long unsigned int i = 0; i < word.size(); i++){
		array[i] = word.at(i);
	}

	cout << "filename: " << array << endl;

	return array;
}

bool isNumber(string word){
	bool isNumber = true;

	for(long unsigned int i = 0; i < word.size(); i++){
		if(isdigit(word.at(i)) == 0){
			isNumber = false;

			break;
		}
	}

	return isNumber;
}

int main(int ARG_COUNT, char* argVect[]){
	int numOfThreads = 0;
	bool outputRedirect = false;
	bool outputRedirectAppend = false;
	bool errorRedirect = false;
	bool errorRedirectAppend = false;
	string fileName = ""; 
	Timer time;

	time.start();

	//If their are six arguments passed to the program we will do the file redirect and check for the -nolock command.
	if(ARG_COUNT == 6){
		if(isNumber(argVect[1]) == true){
			if((stoi(argVect[1]) < 2) || (stoi(argVect[1]) > 10000)){
				cerr << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

				exit(-1);
			}

			else{
				numOfCollatz = stoi(argVect[1]);

				for(int i = 0; i < numOfCollatz; i++){
					computedTimes.push_back(0);
				}
			}
		}

		else{
			cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}

		if(isNumber(argVect[2]) == true){
			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = stoi(argVect[2]);
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

		//FIXME mayebe file names have a specific structure to follow, do we check?
		fileName = argVect[5];

		if(outputRedirect == true){
			freopen(cStringToCxx(fileName), "w", stdout);
		}

		else if(errorRedirect == true){
			freopen(cStringToCxx(fileName), "w", stderr);
		}

		else if(outputRedirectAppend == true){
			freopen(cStringToCxx(fileName), "a", stdout);
		}

		else if(errorRedirectAppend == true){
			freopen(cStringToCxx(fileName), "a", stderr);
		}
	}

	else if(ARG_COUNT == 4){
		if(isNumber(argVect[1]) == true){
			if((stoi(argVect[1]) < 2) || (stoi(argVect[1]) > 10000)){
				cerr << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

				exit(-1);
			}

			else{
				numOfCollatz = stoi(argVect[1]);

				for(int i = 0; i < numOfCollatz; i++){
					computedTimes.push_back(0);
				}
			}
		}

		else{
			cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}

		if(isNumber(argVect[2]) == true){
			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: the number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = stoi(argVect[2]);
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
		if(isNumber(argVect[1]) == true){
			if((stoi(argVect[1]) < 2) || (stoi(argVect[1]) > 10000)){
				cerr << "ERROR: Only numbers between 2 and 10000 are acceptable." << endl;

				exit(-1);
			}

			else{
				numOfCollatz = stoi(argVect[1]);

				for(int i = 0; i < numOfCollatz; i++){
					computedTimes.push_back(0);
				}
			}
		}

		else{
			cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;

			exit(-1);
		}
	
		if(isNumber(argVect[2]) == true){
			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = stoi(argVect[2]);
			}
		}

		else{
			cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;

			exit(-1);
		}
	}

	thread threads[numOfThreads];

	for(int i = 0; i < numOfThreads; i++){
		threads[i] = thread(threadInstructions);
	}

	computedTimes.at(0) = computeCollatz(1);

	for(int i = 0; i < numOfThreads; i++){
		threads[i].join();
	}

	delete array;

	time.stop();

	for(long unsigned int i = 0; i < computedTimes.size(); i++){
		cout << i + 1 << "," << computedTimes.at(i) << endl;
	}

	cerr << numOfCollatz << ", " << numOfThreads << ", " << time.getElapsedTime() << endl;
	
	return 0;
}
