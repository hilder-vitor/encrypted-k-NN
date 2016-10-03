/*
   Class to model a dataset to HomomorphicWeightedkNN
*/

#include "EncryptedDatasetWeighted.h"
#include "lib/yashe/src/vectorutils.h"

using namespace NTL;
using namespace std;

vector<ZZ> EncryptedDatasetWeighted::encrypt_vector(const DataInstance& sample){
	vector<ZZ> data;
	unsigned int P = sample.size();
	for (unsigned int i = 0; i < P; i++){
		data.push_back(ope.encrypt(sample[i]));
	}
	return data;
}

/** XXX:  hardcoded 64 should be <gap>
 *  XXX:  it can be improved pre-calculating all the possible encoded values. 
 */
mpz_class EncryptedDatasetWeighted::encode_class(unsigned int i){
	if (i >= number_of_classes){
		cerr << "trying to encode value " << i << ", which is greater than the number of classes " << number_of_classes << endl;
		exit(5);
	}
	//mpz_class position = 1 << i*64;
	mpz_class position;
	mpz_mul_2exp (position.get_mpz_t(), mpz_class(1).get_mpz_t(), i*64);

	return position;
}



EncryptedDataInstance EncryptedDatasetWeighted::encrypt_training_instance(const DataInstance& sample){
	vector<ZZ> data = encrypt_vector(sample);
	paillier::Ciphertext enc_class = paillier.enc(encode_class(sample.get_class()));
	return EncryptedDataInstance(sample.get_id(), data, enc_class);
}

void EncryptedDatasetWeighted::encrypt_training_data(vector<DataInstance> data){
	unsigned int N = data.size();
	for (unsigned int i = 0; i < N; i++){
		training_data.push_back(encrypt_training_instance(data[i]));
	}
}

void EncryptedDatasetWeighted::encrypt_testing_data(vector<DataInstance> data){
	unsigned int N = data.size();
	for (unsigned int i = 0; i < N; i++){
		EncryptedDataInstance edi(data[i].get_id(), encrypt_vector(data[i]), zero);
		testing_data.push_back(edi);
	}
}


EncryptedDatasetWeighted::EncryptedDatasetWeighted(const Dataset& plain_dataset, OPE& _ope, paillier::Paillier& _paillier)
	: ope(_ope), paillier(_paillier), number_of_classes(plain_dataset.number_of_classes) {

	mpz_class plain_zero(0);
	zero = paillier.enc(plain_zero);

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


unsigned int EncryptedDatasetWeighted::number_of_training_instances(){
	return training_data.size();
}

unsigned int EncryptedDatasetWeighted::number_of_testing_instances(){
	return testing_data.size();
}

unsigned int EncryptedDatasetWeighted::instances_dimensions(){
	return training_data[0].size();
}

std::ostream& operator<<(std::ostream& os, const EncryptedDatasetWeighted& data){
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
