#include "lib/ope/lib/ope.hh"
#include "lib/yashe/src/vectorutils.h"
#include "lib/yashe/src/timing.h"

#include"Dataset.h"
#include"EncryptedDataset.h"
#include"EncryptedDataInstance.h"
#include"HomomorphicKnn.h"

#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;


int decrypt_assigned_class(paillier::Ciphertext enc_class, paillier::Paillier& paillier, unsigned int number_of_classes, unsigned int gap, mpz_class two_to_gap){
	mpz_class plain_class = paillier.dec(enc_class);
	int index_max = 0;
	mpz_class value_max = plain_class % two_to_gap;
	for (int i = 1; i <= number_of_classes; i++){
//		mpz_class tmp = (plain_class >> i*gap) % gap;
		mpz_class tmp;
		mpz_tdiv_q_2exp(tmp.get_mpz_t(), plain_class.get_mpz_t(), i*gap);
		tmp = tmp % two_to_gap;
		if (tmp > value_max){
			index_max = i;
			value_max = tmp;
		}
	}
	return index_max;
}

int main(int argc, char **argv) {
	
	if (argc < 3){
		cout << "ERROR: usage\n   " << argv[0] << " <data_set> <k>" << endl;
		return 0;
	}

	string dataset_name = argv[1];
	// number of neighbours in the k-NN
	unsigned int k = stoi(argv[2]);

	// plaintext range's length in bits (plaintexts are in [0, 2**P[
	unsigned int P = 32;
	// ciphertext range's length in bits (ciphertexts are in [0, 2**C[
	unsigned int C = 40;
	OPE ope("A_ v3Ry $TR0NG Key", P, C);

	cout << "Generating keys." << endl;
	paillier::Paillier paillier(1024);
	cout << "Keys generated." << endl;

	unsigned int gap = 64;
	mpz_class two_to_gap("18446744073709551616");

	Dataset data(dataset_name);
	cout << data << endl;
	cout << "# classes: " << data.number_of_classes << endl;
	vector<int> hits_by_class(data.number_of_classes + 1, 0);
	vector<int> errors_by_class(data.number_of_classes + 1, 0);

	timing timing;
	EncryptedDataset enc_data(data, ope, paillier);

	HomomorphicKnn knn(k, enc_data.training_data, paillier);

	unsigned int right = 0;
	unsigned int wrong = 0;
	unsigned int total_test_cases = enc_data.testing_data.size();

	for (unsigned int j = 0; j < total_test_cases; j++){
		timing.start();
		paillier::Ciphertext encrypted_class = knn.classify(enc_data.testing_data[j]);
		timing.stop("time homomorphic classification");
		unsigned int assigned_class = decrypt_assigned_class(encrypted_class, paillier, data.number_of_classes, gap, two_to_gap);
		unsigned int expected_class = data.testing_data[j].get_class();
		cout << "instance #"<< j << ": assigned class: " << assigned_class << endl; 
		cout << "instance #"<< j << ": expected class: " << expected_class << endl;
		cout << endl;
		if (assigned_class == expected_class){
			right++;
			hits_by_class[expected_class]++;
		}else{
			wrong++;
			errors_by_class[expected_class]++;
		}
	}
//	timing_whole_classification.stop("Time to the whole classification");
	timing.show();
	cout << "accuracy: " << ((double) right) / total_test_cases << endl;

	for (unsigned int i = 1; i < hits_by_class.size(); i++){
		unsigned int total_per_class = hits_by_class[i] + errors_by_class[i];
		if (total_per_class == 0)
			cout << "  class " << i << ":  ---" << endl;
		else
			cout << "  class " << i << ":";
			cout << " hits: " << hits_by_class[i] << "; errors: " << errors_by_class[i] << "; accuracy: " << ((double) hits_by_class[i]) / total_per_class << endl;
	}

	return 0;
}
