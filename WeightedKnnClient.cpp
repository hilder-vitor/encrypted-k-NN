#include "lib/ope/lib/ope.hh"
#include "lib/yashe/src/Yashe.h"
#include "lib/yashe/src/vectorutils.h"
#include "lib/yashe/src/timing.h"

#include"Dataset.h"
#include"EncryptedDataset.h"
#include"EncryptedDataInstance.h"
#include"HomomorphicWeightedKnn.h"

#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;


/******************************************************************************
 * 		FIVE CLASS   (  =>  number of slots = 5)
 * 		K = 9  (  =>  t_i >= 18)
 ******************************************************************************/
vector<fmpzxx> ts = {fmpzxx("32"), fmpzxx("27"), fmpzxx("19"), fmpzxx("23"), fmpzxx("29")};



// -------------------------------------------------------------------------------
// --------- PARAMS FOR L = 2 (or L=1) WITH t = 350064 (about 2**19) --------------------------------
// -------------------------------------------------------------------------------
const struct YASHEParams params_k9_class5 = { 
//	4793, // n = 4793  (totient(n) = 4972 is the degree of the cyclotomic polynomial) L=2
	3109, // n = 3109  (totient(n) = 3108 is the degree of the cyclotomic polynomial) L=1
	1,  // Key distribution's standard deviation
	8,  // Error distribution's standard deviation
//	fmpzxx("196159429230833773869868419475239575503198607639501078831"), // q  (188-bit prime) L=2
	fmpzxx("1267650600228229401496703205653"), // q  (100-bit prime)   Ciphertext: ring R/qR   L=1
//	fmpzxx("21267647932558653966460912964485513283"), // q  (124-bit prime)   Ciphertext: ring R/qR   L=1
//	fmpzxx("350064") // t = 16*9*11*13*17 
	fmpzxx("10949472") // t = 32*27*19*23*29
};


// -------------------------------------------------------------------------------
// --------- PARAMS FOR L = 4 WITH t = 2**32 (4294967296) --------------------------------
// -------------------------------------------------------------------------------
const struct YASHEParams params_L4_t2to32 = { 
	16384, // n = 2**14  (totient(n) = 2**13 is the degree of the cyclotomic polynomial)
	1,  // Key distribution's standard deviation
	8,  // Error distribution's standard deviation
	fmpzxx("2839213766779714416208296124562517712318911565184836172974571090549372219192960637992933791850638927971728600024477257552869537611963"), // q  (440-bit prime)   Ciphertext: ring R/qR 
	fmpzxx("4294967296") // t: Cleartext:  ring R/(moduli)R  (for instance, if t = 7, R/7R) 
};

int decrypt_assigned_class(const RealNumberCiphertext& enc_class, Yashe& yashe, CoefficientwiseCRT& crt){
	vector<Plaintext> unpacked_class = crt.unpack(yashe.decrypt(enc_class));
	unsigned int assigned_class = 0;
	double max_value = -1;
	for (unsigned int i = 0; i < unpacked_class.size(); i++){
		yashe.t = ts[i];
		double value = yashe.decode(RealNumberPlaintext(unpacked_class[i], 64));
		if (value > max_value){
			max_value = value;
			assigned_class = i;
		}
	}
	yashe.t = crt.get_modulus();
	return assigned_class;
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
	unsigned int C = 33;
	OPE ope("A__v3Ry $TR0NG Key", P, C);

	Yashe* yashe;
	string fileName("keys/L2_k9_class5.keys");
	std::ifstream inFile(fileName);
	if (inFile.good()){
		cout << "Loading yashe." << endl;
		yashe = new Yashe(fileName);
	}else{
		cout << "Generating keys." << endl;
		yashe = new Yashe(params_k9_class5);
		yashe->serialize(fileName);	
		cout << "Keys saved in " << fileName << endl;
	} 

	fmpz_mod_polyxx cyclotomic = yashe->get_phi();

	Dataset data(dataset_name);
	cout << data << endl;

	CoefficientwiseCRT crt(ts, cyclotomic); 

	timing timing;

	EncryptedDataset enc_data(data, ope, *yashe, crt);

	//cout << "BEGIN: sending encrypted data to the cloud...." << endl;
	HomomorphicWeightedKnn knn(k, enc_data.training_data, *yashe, crt);

	unsigned int right = 0;
	unsigned int wrong = 0;
	unsigned int total_test_cases = enc_data.testing_data.size();

	for (unsigned int j = 0; j < total_test_cases; j++){
		timing.start();
		RealNumberCiphertext encrypted_class = knn.classify(enc_data.testing_data[j]);
		timing.stop("time homomorphic classification");
		unsigned int assigned_class = decrypt_assigned_class(encrypted_class, *yashe, crt);
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
