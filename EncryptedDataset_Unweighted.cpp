/*
   Class to model a dataset to HomomorphickNN
*/

#include "EncryptedDataset_Unweighted.h"
#include "lib/yashe/src/vectorutils.h"

using namespace NTL;
using namespace std;

vector<ZZ> EncryptedDataset_Unweighted::encrypt_vector(const DataInstance& sample){
	vector<ZZ> data;
	unsigned int P = sample.size();
	for (unsigned int i = 0; i < P; i++){
		data.push_back(ope.encrypt(sample[i]));
	}
	return data;
}

Plaintext EncryptedDataset_Unweighted::encode_class(unsigned int i){
	Plaintext plain_class(yashe.t, *(yashe.phi));
	plain_class.set(i, 1);
	return plain_class;
}

EncryptedDataInstance EncryptedDataset_Unweighted::encrypt_training_instance(const DataInstance& sample){
	vector<ZZ> data = encrypt_vector(sample);
	RealNumberCiphertext enc_class = yashe.encrypt(encode_class(sample.get_class()));
	return EncryptedDataInstance(sample.get_id(), data, enc_class);
}

void EncryptedDataset_Unweighted::encrypt_training_data(vector<DataInstance> data){
	unsigned int N = data.size();
	for (unsigned int i = 0; i < N; i++){
		training_data.push_back(encrypt_training_instance(data[i]));
	}
}

void EncryptedDataset_Unweighted::encrypt_testing_data(vector<DataInstance> data){
	RealNumberCiphertext zero = yashe.encrypt(yashe.encode(0.0, 0));
	unsigned int N = data.size();
	for (unsigned int i = 0; i < N; i++){
		EncryptedDataInstance edi(data[i].get_id(), encrypt_vector(data[i]), zero);
		testing_data.push_back(edi);
	}
}


EncryptedDataset_Unweighted::EncryptedDataset_Unweighted(const Dataset& plain_dataset, OPE& _ope, Yashe& _yashe)
	: ope(_ope), yashe(_yashe), number_of_classes(plain_dataset.number_of_classes) {
	timing tm;
	tm.start();
	encrypt_training_data(plain_dataset.training_data);
	tm.stop("Encryption of the training dataset");
	
	tm.start();
	encrypt_testing_data(plain_dataset.testing_data);
	tm.stop("Encryption of the testing dataset");

	if (training_data[0].size() != testing_data[0].size()){
		std::cout << "FAIL: training data and testing data do not have the same number of variables of interest." << std::endl;
		exit(4);
	}
}


unsigned int EncryptedDataset_Unweighted::number_of_training_instances(){
	return training_data.size();
}

unsigned int EncryptedDataset_Unweighted::number_of_testing_instances(){
	return testing_data.size();
}

unsigned int EncryptedDataset_Unweighted::instances_dimensions(){
	return training_data[0].size();
}

std::ostream& operator<<(std::ostream& os, const EncryptedDataset_Unweighted& data){
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
