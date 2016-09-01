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


// -------------------------------------------------------------------------------
// --------- PARAMS FOR L = 0 WITH t = 350064 (about 2**19) --------------------------------
// -------------------------------------------------------------------------------
const struct YASHEParams params_yashe_k_up_to_16 = { 
	541, // n = 541  (totient(n) = 540 is the degree of the cyclotomic polynomial)
	1,  // Key distribution's standard deviation
	8,  // Error distribution's standard deviation
	fmpzxx("33554467"), // q  (25-bit prime)   Ciphertext: ring R/qR
	fmpzxx("16") // t = 16  (maximum number of neighbours to be considered)
};


int decrypt_assigned_class(const Ciphertext& enc_class, Yashe& yashe){
	Plaintext plain_class = yashe.decrypt(enc_class);
	if (plain_class.is_zero())
		return 0;
	int index_max = 0;
	for (int i = 1; i <= plain_class.degree(); i++){
		if (plain_class.get(i) > plain_class.get(index_max))
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

	Yashe* yashe;
	string fileName("keys/params_yashe_k_up_to_16.keys");
	std::ifstream inFile(fileName);
	if (inFile.good()){
		cout << "Loading yashe." << endl;
		yashe = new Yashe(fileName);
		cout << "Loaded." << endl;
	}else{
		cout << "Generating keys." << endl;
		yashe = new Yashe(params_yashe_k_up_to_16);
		yashe->serialize(fileName);	
		cout << "Keys saved in " << fileName << endl;
	} 


	Dataset data(dataset_name);
	cout << data << endl;

	timing timing;
	EncryptedDataset_Unweighted enc_data(data, ope, *yashe);

	HomomorphicKnn knn(k, enc_data.training_data, *yashe);

	unsigned int right = 0;
	unsigned int wrong = 0;
	unsigned int total_test_cases = enc_data.testing_data.size();

	for (unsigned int j = 0; j < total_test_cases; j++){
		timing.start();
		Ciphertext encrypted_class = knn.classify(enc_data.testing_data[j]);
		timing.stop("time homomorphic classification");
		unsigned int assigned_class = decrypt_assigned_class(encrypted_class, *yashe);
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
