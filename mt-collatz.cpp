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

static int range_to_compute_collatz_min = 2;//2 min means cannot ask program to compute collatz sequences for numbers smaller that 2.

mutex mx;
long currentCollatz = 2;
long numOfCollatz = 0;
int numOfThreads = 0;
bool no_lock = false;
vector<long> frequency;
vector<long> computedIterations;
static int COLLATZ_STOPPING_POINT = 1;

/*
 * @param numToCheck
 * @return 0 or any prime number
 * Checks if a number is even.
*/
bool isEven(long numToCheck){
	return numToCheck % 2 == 0;
}

/*
 * @param _1stTerm
 * @return numOfIterations
 * Method below computes the full collatz sequence for the _1stTerm given.
*/
long computeCollatz(long _1stTerm){
	long numOfIterations = 0;
	long valueSoFar = _1stTerm;

	while(valueSoFar != COLLATZ_STOPPING_POINT){//Iterate while not complete.
		//The number is even if its divided by 2, and the result is zero.
		if(isEven(valueSoFar) == true){//If n is even,
			valueSoFar/= 2;
		}

		//The number is odd.
		else{
			valueSoFar = (valueSoFar * 3) + 1;
		}

		numOfIterations++;//Number of steps to compute the collatz sequence.
	}
	
	return numOfIterations;
}

//This is done by every thread.
void threadInstructions(){
	long tempValue = 0;
	long tempCollatz = 0;

	//Iterates through all collatz.
	while(currentCollatz < numOfCollatz){//Keep going until last collatz.
		//Lock variable right here so other threads can't change it at the same time causing undefined behavior.
		if(no_lock == true){//If user wants to avoid locking mechanism.
			//CS.
			tempCollatz = currentCollatz;
			++currentCollatz;
			//END CS.

			if(tempCollatz < numOfCollatz){//same.
				tempValue = computeCollatz(tempCollatz);//same.
				computedIterations.at(tempCollatz) = tempValue;//same.
				frequency.at(tempValue) += 1;//same.

			}
		}

		else{//User wants to keep locking mechanism.
			//Lock.
			mx.lock();
			//CS.
			tempCollatz = currentCollatz;
			++currentCollatz;
			//END CS.
			mx.unlock();
			//END lock.


			if(tempCollatz < numOfCollatz){//same.
				tempValue = computeCollatz(tempCollatz);//same.
				computedIterations.at(tempCollatz) = tempValue;//same.
				mx.lock();//Diff.
				frequency.at(tempValue) += 1;//Same.
				mx.unlock();
			}
		}
	}
}

/*
 * @param word
 * @return isNumber
 * Checks if a string is a number.
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

/*
 * @param last
 * @return true or false
*/
bool isOutOfRange(long last){//Last & greatest number to compute collatz for.
	if(last < range_to_compute_collatz_min){
		return true;
	}

	return false;
}

//Prints this message if the user vialates the checks that are in the above function.
void handleOutOfRangeException(){
	cerr << "ERROR: Only numbers above " << range_to_compute_collatz_min << endl;

	exit(-1);
}

/*
 * @param argNum
*/
void handleNanException(long argNum){
	if(argNum == 1){	
		cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;
	}

	else if(argNum == 2){
		cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;
	}

	exit(-1);
}

/*
 * @return 0
 * The main function.
*/
int main(int ARG_COUNT, char* argVect[]){
	string fileName = ""; 
	Timer time;

	time.start();

	//Arg 1 and 2 error checking routine.
	if(ARG_COUNT == 1){//If only have the file argument.
		cerr << "ERROR: missing arguments required to compute collatz." << endl;
	}

	else if(isOutOfRange(stoi(argVect[1]))){//If user tries to enter an argument in range.
		handleOutOfRangeException();					
	}

	else if(isNumber(argVect[1]) == false){
		handleNanException(1);
	}

	else if(isNumber(argVect[2]) == false){
		handleNanException(2);	
	}

	else{//With some intitial error checking out of the way, proceed with program.
		//Initialize array computedTimes with size numOfCollatz and the num '0'.
		numOfCollatz = stoi(argVect[1]);

		for(long i = 0; i < numOfCollatz; i++){
			computedIterations.push_back(0);
		}

		for(int i = 0; i < 1000; i++){
			frequency.push_back(0);
		}
		
		if(ARG_COUNT == 4){//Handle 4 args.
			numOfThreads = stoi(argVect[2]);

			if(strcmp(argVect[3], "-nolock") == 0){
				no_lock = true;
			}

			else{
				cerr << "ERROR: Expected the \"-nolock\" command." << endl;

				exit(-1);
			}
		}

		else if(ARG_COUNT == 3){//Handle 3 args.
			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = stoi(argVect[2]);
			}
		}

		thread threads[numOfThreads];//Make the threads.

		/*
		Iterate through all threads and pass each thread a starting method.
		This startmethod will:

		1. Compute the collatz for the next number in the range, to compute and increment the number waiting to be computed.
		2. Join the threads together.
		3. Delete the array.
		4. Stop the timer.
		5. Display the computedTimes.
		*/

		//Creates a variable amout of threads, and passes the functions it needs to run.
		for(int i = 0; i < numOfThreads; i++){//Iterate through all threads.
			threads[i] = thread(threadInstructions);//Pass each thread a starting point.
		}

		//Joins the threads with the main thread, which terminates them.
		for(int i = 0; i < numOfThreads; i++){
			threads[i].join();
		}

		//Stop timming the prgram, then proceed to print the calculations. 
		time.stop();

		//Prints out the frequencies of 1 - N - 1.
		for(long unsigned int i = 0; i < frequency.size(); i++){
			cout << i << "," << frequency.at(i) << endl;
		}

		//Print the total time the program took to run till completion, and the inputs passed into the program.
		cerr << numOfCollatz << "," << numOfThreads << "," << time.getElapsedTime() << endl;

		return 0;
	}
}
