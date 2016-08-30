#include "lib/ope/lib/ope.hh"
#include "lib/yashe/src/Yashe.h"
#include "lib/yashe/src/vectorutils.h"
#include "lib/yashe/src/timing.h"

#include"Dataset.h"
#include"EncryptedDataset_Unweighted.h"
#include"EncryptedDataInstance.h"
#include"HomomorphicKnn.h"

#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;


int decrypt_assigned_class(const vector<mpz_class>& enc_class, Paillier& paillier){
	cout << "enc_class = " << enc_class << endl;
	vector<mpz_class> plain_class = paillier.dec(enc_class);
	cout << "plain_class = " << plain_class << endl;
	int index_max = 0;
	for (int i = 1; i <= plain_class.size(); i++){
		if (plain_class[i] > plain_class[index_max])
			index_max = i;
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
	Paillier paillier(2048);
	cout << "Keys generated." << endl;

	Dataset data(dataset_name);
	cout << data << endl;

	timing timing;
	EncryptedDataset_Unweighted enc_data(data, ope, paillier);

	HomomorphicKnn knn(k, enc_data.training_data, paillier);

	unsigned int right = 0;
	unsigned int wrong = 0;
	unsigned int total_test_cases = enc_data.testing_data.size();

	for (unsigned int j = 0; j < total_test_cases; j++){
		timing.start();
		vector<mpz_class> encrypted_class = knn.classify(enc_data.testing_data[j]);
		timing.stop("time homomorphic classification");
		unsigned int assigned_class = decrypt_assigned_class(encrypted_class, paillier);
		cout << "instance #"<< j << ": assigned class: " << assigned_class << endl; 
		cout << "instance #"<< j << ": expected class: " << data.testing_data[j].get_class() << endl;
		cout << endl;
		if (assigned_class == data.testing_data[j].get_class()){
			right++;
		}else{
			wrong++;
		}
	}
//	timing_whole_classification.stop("Time to the whole classification");
	timing.show();
	cout << "accuracy: " << ((double) right) / total_test_cases << endl;

	return 0;
}
