#include "frequency.hpp"

void Frequency::initVector(int size){
	for(int i = 0; i <= size; i++){
		frequencies.push_back(0);
	}
}

vector<int> Frequency::getFrequencies(){
	return frequencies;
}

void Frequency::incrementFrequency(int position){
	int size = frequencies.size();

	if(position < size){
		int tempValue = frequencies.at(position);
		frequencies.at(position) = tempValue + 1;
	}
}
