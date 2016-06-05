#include "lib/ope/lib/ope.hh"
#include "lib/yashe/src/Yashe.h"

#include"Dataset.h"
#include"EncryptedDataset.h"
#include"EncryptedDataInstance.h"

#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;


/******************************************************************************
 * 		FIVE CLASS   (  =>  number of slots = 5)
 * 		K = 9  (  =>  t_i >= 9)
 ******************************************************************************/
vector<fmpzxx> ts = {fmpzxx("16"), fmpzxx("9"), fmpzxx("11"), fmpzxx("13"), fmpzxx("17")};


// -------------------------------------------------------------------------------
// --------- PARAMS FOR L = 2 WITH t = 350064 (about 2**19) --------------------------------
// -------------------------------------------------------------------------------
const struct YASHEParams params_k9_class5_ = { 
	4793, // n = 4793  (totient(n) = 4972 is the degree of the cyclotomic polynomial)
	1,  // Key distribution's standard deviation
	8,  // Error distribution's standard deviation
	fmpzxx("392318858461667547739736838950479151006397215279002157113"), // q  (188-bit prime)   Ciphertext: ring R/qR 
	fmpzxx("350064") // t: Cleartext:  ring R/(moduli)R  (for instance, if t = 7, R/7R) 
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


int main(int argc, char **argv) {
/*	
	if (argc < 2){
		cout << "ERROR: usage\n   " << argv[0] << " <data_set>" << endl;
		return 0;
	}

	string dataset_name = argv[1];
*/
	// plaintext range's length in bits (plaintexts are in [0, 2**P[
	unsigned int P = 32;
	// ciphertext range's length in bits (ciphertexts are in [0, 2**C[
	unsigned int C = 33;
	OPE ope("A_ v3Ry $TR0NG Key", P, C);

	Yashe* yashe;
	string fileName("keys/L4_t2to32_yashe.keys");
	std::ifstream inFile(fileName);
	if (inFile.good()){
		cout << "Loading yashe." << endl;
		yashe = new Yashe(fileName);
	}else{
		cout << "Generating keys." << endl;
		yashe = new Yashe(params_L4_t2to32);
		yashe->serialize(fileName);	
	} 


	Dataset data("simple5x3");

	cout << data << endl;

	EncryptedDataset enc_data(data, ope, *yashe);

	cout << enc_data << endl;
}
