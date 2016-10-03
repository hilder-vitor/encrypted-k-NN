#ifndef __HEKNN__CLOUD_
#define __HEKNN__CLOUD_

#include <vector>
#include <iostream>

#include "EncryptedDataInstance.h"
#include "lib/paillier/src/Paillier.h"


class HomomorphicKnn {
    private:
	unsigned int k;
	std::vector<EncryptedDataInstance> instances;
	paillier::Paillier& paillier;

	// auxiliar functions
	void compute_all_distances(const EncryptedDataInstance& query);
	void sort_by_distance();
	double sum_of_inverse_distances();
	paillier::Ciphertext accumulate_classes(); // vector of (Paillier) ciphertexts

    public:

	HomomorphicKnn(unsigned int _k, const std::vector<EncryptedDataInstance>& _data, paillier::Paillier& pk);

	paillier::Ciphertext classify(const EncryptedDataInstance& query);

	void set_k(unsigned int neighbourhood_size);

};

#endif
