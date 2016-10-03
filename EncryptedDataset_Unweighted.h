#ifndef YASHE__ENC___DATASET__CLASS
#define YASHE__ENC___DATASET__CLASS

/*
   Class to model a dataset to HomomorphickNN
*/

#include "EncryptedDataInstance.h"
#include "DataInstance.h"
#include "Dataset.h"

#include "lib/ope/lib/ope.hh"
#include "lib/paillier/src/Paillier.h"
#include "lib/yashe/src/timing.h"

#include <vector>
#include <iostream>

#include <NTL/ZZ.h>

class EncryptedDataset_Unweighted {
    
	private:
	OPE& ope;
	paillier::Paillier& paillier;
	unsigned int number_of_classes;
	paillier::Ciphertext zero;

	vector<NTL::ZZ> encrypt_vector(const DataInstance& sample);
	mpz_class encode_class(unsigned int class_id);
	EncryptedDataInstance encrypt_training_instance(const DataInstance& sample);
	void encrypt_training_data(vector<DataInstance> data);
	void encrypt_testing_data(vector<DataInstance> data);

    public:
	std::vector<EncryptedDataInstance> training_data; // Data instances whose classes are already know
	std::vector<EncryptedDataInstance> testing_data; // Data instances used to verify the accuracy of the classifier

	EncryptedDataset_Unweighted(const Dataset& plain_dataset, OPE& ope, paillier::Paillier& paillier);

	unsigned int number_of_training_instances();
	unsigned int number_of_testing_instances();

	unsigned int instances_dimensions();
};

std::ostream& operator<<(std::ostream&, const EncryptedDataset_Unweighted&);

#endif
