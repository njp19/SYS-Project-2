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

static int range_to_compute_collatz_min = 2;     // 2 means cannot ask program to compute collatz sequences for numbers between 0 to 1, 0, or anything smaller
static int range_to_compute_collatz_max = 10000; // max means cannot ask to compute collatz sequences for numbers between 0 to 10001 or greater if max = 10000

mutex mx;
int currentCollatz = 2;
int numOfCollatz = 0;
// char* array; FIXME may need to uncomment
bool no_lock = false;
vector<int> computedTimes;
static int COLLATZ_STOPPING_POINT = 1;


bool isEven(int numToCheck){
	return numToCheck % 2 == 0;
}


// method below computes the full collatz sequence for the _1stTerm given
int computeCollatz(int _1stTerm){
	int numOfIterations = 0;
	int valueSoFar = 1;

	if(_1stTerm != COLLATZ_STOPPING_POINT){ // done if sequence computation complete
		while(valueSoFar != COLLATZ_STOPPING_POINT){ // iterate while not complete

			//The number is even if its divided by 2, and the result is zero.
			if(	isEven(valueSoFar) ) { // if n is even,
				valueSoFar /= 2; 				 // n/2
			}

			//The number is negative. FIXME did you mean "The number is odd", Adiessa?
			else{
				valueSoFar = (valueSoFar * 3) + 1;
			}	

			numOfIterations++; // frequency counter
		}
	}
	// FIXME this doesn't seem right
	else{ // _1stTerm is a stopping point - no sequence exists
		/*
		valueSoFar = (valueSoFar * 3) + 1;

		while(valueSoFar != 1){
			// The number is even if it's divided by 2, and the result is zero.
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
		*/
	}
	return numOfIterations;
}


void threadInstructions(){

	// code block
	// iterates through all collatz
	while(currentCollatz <= numOfCollatz){ // keep going until last collatz
		// computedTimes.at(currentCollatz - 1) /* <-  */ = /*  -> */ computeCollatz(currentCollatz);

		//Lock variable right here so other threads can't change it at the same time causing undefined behavior.
		if(no_lock == true){ 	// if user wants to avoid locking mechanism
			
			// Critical Section
			computedTimes.at(currentCollatz - 1) /* <-  */ = /*  -> */ computeCollatz(currentCollatz);
			currentCollatz++;
		}
		else { // user wants to keep locking mechanism
			mx.lock();

			// Critical Section
			computedTimes.at(currentCollatz - 1) /* <-  */ = /*  -> */ computeCollatz(currentCollatz);
			currentCollatz++;
			
			mx.unlock();
		}
	}
}

/* FIXME may need to uncomment this if need freopen stuff
char* cStringToCxx(string word){
	array = new char[word.size()];

	for(long unsigned int i = 0; i < word.size(); i++){
		array[i] = word.at(i);
	}

	cout << "filename: " << array << endl;

	return array;
}
*/

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


bool isOutOfRange(int last /* last & greatest number to compute collatz for */ ) {
	if((last < range_to_compute_collatz_min) || last > range_to_compute_collatz_max){
		return true;
	}
	return false;
}


void handleOutOfRangeException(){
	cerr << "ERROR: Only numbers between " << range_to_compute_collatz_min << " and " << range_to_compute_collatz_max << " are acceptable." << endl;

	exit(-1);
}


void handleNanException(int argNum){
	if(argNum == 1){	
		cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;
	}
	else if(argNum == 2){
		cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;
	}

	exit(-1);
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

	// arg 1 and 2 error checking routine
	if(ARG_COUNT == 1){ // if only have the file argument
		cerr <<"ERROR: missing arguments required to compute collatz." << endl;
	}
	else if( isOutOfRange( stoi(argVect[1]) ) ){ // if user tries to enter an argument in range
		handleOutOfRangeException();					
	}
	else if( isNumber(argVect[1]) == false ) {
		handleNanException(1);
	}	
	else if( isNumber(argVect[2]) == false ){
		handleNanException(2);	
	}
	else{ // with some intitial error checking out of the way, proceed with program

		// initialize array computedTimes with size numOfCollatz and the num '0'
		numOfCollatz = stoi(argVect[1]);
		for(int i = 0; i < numOfCollatz; i++){
			computedTimes.push_back(0); 		
		}

	

		//If their are six arguments passed to the program we will do the file redirect and check for the -nolock command.
		if(ARG_COUNT == 6){ // handle 6 args	

			// handle arg 2
			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}
			else{
				numOfThreads = stoi(argVect[2]);
			}

			// handle arg 3
			if(strcmp(argVect[3], "-nolock") == 0){
				no_lock = true; // wants no locking mechanism
			}
			else{
				cerr << "ERROR: Expected \"-nolock\" command." << endl;

				exit(-1);
			}

			// handle arg 4
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

			// handle arg 5
			//FIXME mayebe file names have a specific structure to follow, do we check?
			fileName = argVect[5];

			
	//		if(outputRedirect == true){
		/*		freopen(cStringToCxx(fileName), "w", stdout);
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
			*/
		}
		
		if(ARG_COUNT == 4){ // handle 4 args
	
			numOfThreads = stoi(argVect[2]);

			if(strcmp(argVect[3], "-nolock") == 0){
				no_lock = true;
			}
			else{
				cerr << "ERROR: Expected the \"-nolock\" command." << endl;

				exit(-1);
			}
		}
		else if(ARG_COUNT == 3){	// handle 3 args

			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}
			else{
				numOfThreads = stoi(argVect[2]);
			}

		}



		// code below is
		// common to any number of arguments
		thread threads[numOfThreads]; // make the threads

		// iterate through all threads and pass each thread a starting method
		// this startmethod will:
		// 1. Compute the collatz for the next number in the range
		//    to compute and incremente the number waiting to be computed
		// 2. Join the threads together FIXME
		// 3. delete the array;
		// 4. stop the timer
		// 5. display the computedTimes
		for(int i = 0; i < numOfThreads; i++){ // iterate through all threads
			threads[i] = thread(threadInstructions); // pass each thread a starting point
		}

		computedTimes.at(0) = computeCollatz(1);

		for(int i = 0; i < numOfThreads; i++){
			threads[i].join();
		}

		// delete array; FIXME may need to uncomment this if need freopen stuff

		time.stop();

		// 5
		for(long unsigned int i = 0; i < computedTimes.size(); i++){
			cout << i + 1 << "," << computedTimes.at(i) << endl;
		}

		cerr << numOfCollatz << ", " << numOfThreads << ", " << time.getElapsedTime() << endl;

		return 0;
	}
}
