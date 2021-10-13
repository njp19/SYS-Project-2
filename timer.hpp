/*
 * @author Adiessa Bell and Nathanael Pelling
 * @date 10/12/2021
 * @info course COP4634
*/

#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <ctime>
#include <chrono>

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
