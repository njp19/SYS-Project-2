/*
 * @author Adiessa Bell and Nathanael Pelling
 * @date 10/12/2021
 * @info course COP4634
*/

#include "timer.hpp"

//Starts the timer by getting the current time from the system.
void Timer::start(){
	startTime = system_clock::now();
}

//Does not stop the timer just gets the current time, and then subtract from it the start time to get the elapsed time.
void Timer::stop(){
	endTime = system_clock::now();
	elapsedTime = endTime - startTime;
}

//Returns the elapsed time
double Timer::getElapsedTime(){
	return elapsedTime.count();
}
