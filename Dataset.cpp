#include "Dataset.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

void load_data(const std::string& filename, vector<DataInstance>& destination){
	unsigned int N;
	unsigned int P;
	unsigned int d;
	bool passed = false;

	FILE* file = fopen(filename.c_str(), "r");

	if (! file){
		std::cout << "ERROR: file " << filename << " was not found." << std::endl;
		exit(2);
	}

	if (fscanf(file, "%u", &d)){
		N = d;
		if (fscanf(file, "%u", &d)){
			P = d;
			passed = true;
		}
	}

	if (! passed){
		std::cout << "FAIL: values inside the file are not the expected ones" << std::endl;
		exit(3);
	}

	destination.resize(N);
	vector<int> data_single_instance(P);

	// read the datamatrix
	for(unsigned int i = 0; i < N; i++){
		for(unsigned int j = 0; j < P; j++){
			if(fscanf(file, "%d", &d))
				data_single_instance[j] = d;
			else
				passed = false;
		}
		// read the class
		fscanf(file, "%d", &d);
		destination[i] = DataInstance(i, data_single_instance, d);
	}

	if (! passed){
		std::cout << "FAIL: could not read the values inside the file." << std::endl;
		exit(4);
	}
}

/* Constructor
 *
 * 	Try to read a file and create an instance of the dataset
 */
Dataset::Dataset(const std::string& filename){
	load_data(filename + "_training", training_data);
	load_data(filename + "_testing", testing_data);
	if (training_data[0].size() != testing_data[0].size()){
		std::cout << "FAIL: training data and testing data do not have the same number of variables of interest." << std::endl;
		exit(4);
	}
}

unsigned int Dataset::number_of_training_instances(){
	return training_data.size();
}

unsigned int Dataset::number_of_testing_instances(){
	return testing_data.size();
}

unsigned int Dataset::instances_dimensions(){
	return training_data[0].size();
}

std::ostream& operator<<(std::ostream& os, const Dataset& data){
	os << "Training:" << endl;
	os << data.training_instances() << endl;
	os << "Testing:" << endl;
	os << data.testing_instances() << endl;
	
	return os;
}
