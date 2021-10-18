/*
 * @author Adiessa Bell and Nathanael Pelling
 * @date 10/12/2021
 * @info course COP4634
*/

//Makes sure that this class isnt double included.
#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <ctime>
#include <chrono>

/*
 * Class that is a timer that measures the duration of the program.
 * See Timer.cpp. It has detailed explanation of tthe contents here.
*/
using namespace std::chrono;

class Timer{
	private:
		time_point<system_clock> startTime;
		time_point<system_clock> endTime;
		duration<double> elapsedTime;

	public:
		void start();
		void stop();
		double getElapsedTime();
};

#endif
