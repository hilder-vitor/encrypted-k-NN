/*
   Class to model a dataset to HomomorphickNN
*/

#include "EncryptedDataset.h"
#include "lib/yashe/src/vectorutils.h"

using namespace NTL;
using namespace std;

vector<ZZ> EncryptedDataset::encrypt_vector(const DataInstance& sample){
	vector<ZZ> data;
	unsigned int P = sample.size();
	for (unsigned int i = 0; i < P; i++){
		data.push_back(ope.encrypt(sample[i]));
	}
	return data;
}

RealNumberPlaintext EncryptedDataset::encode_class(unsigned int i){
	if (i >= number_of_classes){
		cerr << "trying to encode value " << i << ", which is greater than the number of classes " << number_of_classes << endl;
		exit(5);
	}
	vector<Plaintext> plain_vec;
	const vector<fmpzxx>& ts = crt.get_coprimes();
	for (unsigned int j = 0; j < ts.size(); j++){
		fmpzxx tmp_t(ts[j]);
		yashe.t = tmp_t;
		if (j == i)
			plain_vec.push_back(yashe.encode(1.0, 0));
		else
			plain_vec.push_back(yashe.encode(0.0, 0));
	}
	fmpzxx original_t(crt.get_modulus());
	yashe.t = original_t;
	RealNumberPlaintext plain_class(crt.pack(plain_vec), 0);
	return plain_class;
}

EncryptedDataInstance EncryptedDataset::encrypt_training_instance(const DataInstance& sample){
	vector<ZZ> data = encrypt_vector(sample);
	RealNumberCiphertext enc_class = yashe.encrypt(encode_class(sample.get_class()));
	return EncryptedDataInstance(sample.get_id(), data, enc_class);
}

void EncryptedDataset::encrypt_training_data(vector<DataInstance> data){
	unsigned int N = data.size();
	for (unsigned int i = 0; i < N; i++){
		training_data.push_back(encrypt_training_instance(data[i]));
	}
}

void EncryptedDataset::encrypt_testing_data(vector<DataInstance> data){
	RealNumberCiphertext zero = yashe.encrypt(yashe.encode(0.0, 0));
	unsigned int N = data.size();
	for (unsigned int i = 0; i < N; i++){
		EncryptedDataInstance edi(data[i].get_id(), encrypt_vector(data[i]), zero);
		testing_data.push_back(edi);
	}
}


EncryptedDataset::EncryptedDataset(const Dataset& plain_dataset, OPE& _ope, Yashe& _yashe, CoefficientwiseCRT& _crt)
	: ope(_ope), yashe(_yashe), crt(_crt), number_of_classes(plain_dataset.number_of_classes) {
	encrypt_training_data(plain_dataset.training_data);
	encrypt_testing_data(plain_dataset.testing_data);

	if (training_data[0].size() != testing_data[0].size()){
		std::cout << "FAIL: training data and testing data do not have the same number of variables of interest." << std::endl;
		exit(4);
	}

	if (number_of_classes > crt.get_coprimes().size()){
		std::cout << "FAIL: number of possible classes cannot be greater than the number of coprimes elements to CRT (slots in the plaintext)." << std::endl;
		exit(6);
	}
}


unsigned int EncryptedDataset::number_of_training_instances(){
	return training_data.size();
}

unsigned int EncryptedDataset::number_of_testing_instances(){
	return testing_data.size();
}

unsigned int EncryptedDataset::instances_dimensions(){
	return training_data[0].size();
}

std::ostream& operator<<(std::ostream& os, const EncryptedDataset& data){
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
