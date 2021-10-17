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
#include "frequency.hpp"

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
static int range_to_compute_collatz_max = 1000000;//max means cannot ask program to compute collatz sequences for numbers greater than 10000.

mutex mx;
long currentCollatz = 2;
long numOfCollatz = 0;
int numOfThreads = 0;
//Frequency histogram;
//char* array; FIXME may need to uncomment.
bool no_lock = false;
vector<long> frequency;
vector<long> computedIterations;
static int COLLATZ_STOPPING_POINT = 1;

bool isEven(long numToCheck){
	return numToCheck % 2 == 0;
}

//Method below computes the full collatz sequence for the _1stTerm given.
long computeCollatz(long _1stTerm){
	long numOfIterations = 0;
	long valueSoFar = _1stTerm;

	//if(_1stTerm != COLLATZ_STOPPING_POINT){//Done if sequence computation complete.
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

		//histogram.incrementFrequency(numOfIterations);

		/*
		if(numOfIterations < numOfCollatz){
			frequency.at(numOfIterations) += 1;
		}
		*/
	//}

	/*
	//FIXME this doesn't seem right.
	else{//_1stTerm is a stopping point - no sequence exists.
		valueSoFar = 4;
		numOfIterations++;

		while(valueSoFar != 1){
			// The number is even if it's divided by 2, and the result is zero.
			if(valueSoFar % 2 == 0){
				valueSoFar/= 2;
			}

			//The number is odd.
			else{
				valueSoFar = (valueSoFar * 3) + 1;
			}

			numOfIterations++;
		}

		frequency.at(numOfIterations) = frequency.at(numOfIterations) + 1;
	}
	*/
	
	return numOfIterations;
}

void threadInstructions(){
	long tempValue = 0;
	long tempCollatz = 0;

	//Iterates through all collatz.
	while(currentCollatz < numOfCollatz){//Keep going until last collatz.
		
		//Lock variable right here so other threads can't change it at the same time causing undefined behavior.
		if(no_lock == true){//If user wants to avoid locking mechanism.
			

			// CS
			tempCollatz = currentCollatz;
			++currentCollatz;
			// END CS



			if(tempCollatz < numOfCollatz){ // same
				tempValue = computeCollatz(tempCollatz); // same
				computedIterations.at(tempCollatz) = tempValue; // same


				//if(tempValue < numOfCollatz){
					frequency.at(tempValue) += 1; // same
				//}

			}
		}

		else{//User wants to keep locking mechanism.

			// lock
			mx.lock();

			// CS
			tempCollatz = currentCollatz;
			++currentCollatz;
			// END CS

			mx.unlock();
			// END lock


			if(tempCollatz < numOfCollatz){ // same
				tempValue = computeCollatz(tempCollatz); // same
				computedIterations.at(tempCollatz) = tempValue; // same

				mx.lock(); // diff

				//if(tempValue < numOfCollatz){
					frequency.at(tempValue) += 1; // same
				//}

				mx.unlock();
			}
		}
	}
}

/* FIXME may need to uncomment this if need freopen stuff.
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


bool isOutOfRange(long last){//Last & greatest number to compute collatz for.
	if((last < range_to_compute_collatz_min) || (last > range_to_compute_collatz_max)){
		return true;
	}

	return false;
}


void handleOutOfRangeException(){
	cerr << "ERROR: Only numbers between " << range_to_compute_collatz_min << " and " << range_to_compute_collatz_max << " are acceptable." << endl;

	exit(-1);
}


void handleNanException(long argNum){
	if(argNum == 1){	
		cerr << "ERROR: Expected the number of collatz sequences to compute." << endl;
	}

	else if(argNum == 2){
		cerr << "ERROR: Expected the number of threads needed to compute the collatz sequences." << endl;
	}

	exit(-1);
}

long searchVector(long value){
	long frequency = 0;

	if(value != 0){
		for(long unsigned int i = 0; i < computedIterations.size(); i++){
			if(computedIterations.at(i) == value){
				frequency++;
				//This is to make sure numbers are not checked more than once. -1 because its not in the computedIterations vector.
				computedIterations.at(i) = -1;
			}
		}
	}

	return frequency;
}

int main(int ARG_COUNT, char* argVect[]){
	//long numOfThreads = 0;
	/*
	bool outputRedirect = false;
	bool outputRedirectAppend = false;
	bool errorRedirect = false;
	bool errorRedirectAppend = false;
	*/
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
		//histogram.initVector(numOfCollatz);

		for(long i = 0; i < numOfCollatz; i++){
			computedIterations.push_back(0);
			//frequency.push_back(0);
		}

		for(int i = 0; i < 1000; i++){
			frequency.push_back(0);
		}

		//If their are six arguments passed to the program we will do the file redirect and check for the -nolock command.
		if(ARG_COUNT == 6){//Handle 6 args.
			//Handle arg 2.
			if(stoi(argVect[2]) > numOfCollatz){
				cerr << "ERROR: The number of threads cannot exceed the number of collatz sequences to compute." << endl;

				exit(-1);
			}

			else{
				numOfThreads = stoi(argVect[2]);
			}

			//Handle arg 3.
			if(strcmp(argVect[3], "-nolock") == 0){
				no_lock = true;//Wants no locking mechanism.
			}

			else{
				cerr << "ERROR: Expected \"-nolock\" command." << endl;

				exit(-1);
			}

			//Handle arg 4.
			/*
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
			*/

			//Handle arg 5.
			//FIXME mayebe file names have a specific structure to follow, do we check?
			fileName = argVect[5];

			/*
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
			*/
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

		for(int i = 0; i < numOfThreads; i++){//Iterate through all threads.
			threads[i] = thread(threadInstructions);//Pass each thread a starting point.
		}

		for(int i = 0; i < numOfThreads; i++){
			threads[i].join();
		}

		//Delete array; FIXME may need to uncomment this if need freopen stuff

		time.stop();

		/*
		for(int i = 0; i < numOfCollatz; i++){
			frequency.at(i) = searchVector(i);
		}
		*/

		for(long unsigned int i = 0; i < frequency.size(); i++){
			cout << i << "," << frequency.at(i) << endl;
		}

		/*
		for(long unsigned int i = 0; i < computedIterations.size(); i++){
			cout << i << "," << computedIterations.at(i) << endl;
		}
		*/

		/*
		//Where I started implementing frequency counter.
		int count = 0;
		int x = 3;//x should be i of the outer loop that encapsulates this function. So two loops total.

		for(long unsigned int i = 0; i < computedTimes.size(); i++){
			if(computedTimes.at(i) == x){
				count++;
			}
		}
		*/

		cerr << numOfCollatz << "," << numOfThreads << "," << time.getElapsedTime() << endl;

		return 0;
	}
}
