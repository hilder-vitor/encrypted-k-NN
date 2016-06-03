#include "lib/ope/lib/ope.hh"
#include "lib/yashe/src/Yashe.h"

#include"Dataset.h"

#include <NTL/ZZ.h>

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
	OPE o("A_ v3Ry $TR0NG Key", P, C);

	Dataset data("simple5x3");

	cout << data << endl;


}
