#ifndef YASHE__DATASET__CLASS
#define YASHE__DATASET__CLASS



/*
   Class to model a dataset to kNN
*/

#include <vector>
#include <iostream>

class Dataset {
    
    public:
	std::vector<DataInstance> training_data; // Data instances whose classes are already know
	std::vector<DataInstance> testing_data; // Data instances used to verify the accuracy of the classifier

	Dataset(const std::string& filename);

	unsigned int number_of_training_instances();
	unsigned int number_of_testing_instances();

	unsigned int instances_dimensions();

	const vector<DataInstance>& Dataset::number_of_training_instances();

	const vector<DataInstance>& Dataset::number_of_testing_instances();

};	

friend std::ostream& operator<<(std::ostream&, const Dataset&);

#endif
