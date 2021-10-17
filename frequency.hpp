#ifndef FREQUENCY_HPP
#define FREQUENCY_HPP

#include <vector>

using std::vector;

class Frequency{
	private:
		vector<int> frequencies;

	public:
		void initVector(int);
		vector<int> getFrequencies();
		void incrementFrequency(int);
};

#endif
