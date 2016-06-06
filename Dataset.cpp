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

	destination.reserve(N);
	vector<int> data_single_instance(P);

	// read the datamatrix
	for(unsigned int i = 0; i < N; i++){
		for(unsigned int j = 0; j < P; j++){
			if(fscanf(file, "%d", &d))
				data_single_instance[j] = d;
			else{
				passed = false;
			}
		}
		// read the class
		fscanf(file, "%d", &d);
		destination.push_back(DataInstance(i, data_single_instance, d));
	}

	if (! passed){
		std::cout << "FAIL: could not read the values inside the file." << std::endl;
		exit(4);
	}
}

unsigned int max_class(const vector<DataInstance>& instances){
	unsigned int m = 0;
	for (unsigned int i = 0; i < instances.size(); i++){
		if (m < instances[i].get_class()){
			m = instances[i].get_class();
		}
	}
	return m;
}

/* Constructor
 *
 * 	Try to read a file and create an instance of the dataset
 */
Dataset::Dataset(const std::string& filename){
	load_data(filename + ".training", training_data);
	load_data(filename + ".testing", testing_data);
	if (training_data[0].size() != testing_data[0].size()){
		std::cout << "FAIL: training data and testing data do not have the same number of variables of interest." << std::endl;
		exit(4);
	}
	unsigned int classes = max_class(training_data) + 1;
	number_of_classes = max_class(testing_data) + 1;
	if (classes > number_of_classes){
		number_of_classes = classes;
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
	unsigned int N = data.training_data.size();
	unsigned int P = data.training_data[0].size();
	os << "Training:  (" << N << "x" << P << ")" << endl;
	for (unsigned int i = 0; i < N; i++)
		os << data.training_data[i] << endl;


	N = data.testing_data.size();
	os << "Testing:  (" << N << "x" << P << ")" << endl;
	for (unsigned int i = 0; i < N; i++)
		os << data.testing_data[i] << endl;
	
	return os;
}
