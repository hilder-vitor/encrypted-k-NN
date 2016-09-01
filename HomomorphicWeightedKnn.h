#ifndef __HEKNN__CLOUD_
#define __HEKNN__CLOUD_

#include <vector>
#include <iostream>

#include "EncryptedDataInstance.h"
#include "lib/paillier/src/Paillier.h"
#include "lib/yashe/src/vectorutils.h"


class HomomorphicWeightedKnn {
    private:
	unsigned int k;
	std::vector<EncryptedDataInstance> instances;
	Paillier& paillier;
	int number_of_classes;

	// auxiliar functions
	void compute_all_distances(const EncryptedDataInstance& query);
	void sort_by_distance();
	double sum_of_inverse_distances();
	std::vector<mpz_class> accumulate_classes();

	std::vector<mpz_class> encode_weight(EncryptedDataInstance instance, double total);
	
    
    public:

	HomomorphicWeightedKnn(unsigned int _k, const std::vector<EncryptedDataInstance>& _data, Paillier& pk);

	std::vector<mpz_class> classify(const EncryptedDataInstance& query);

	void set_k(unsigned int neighbourhood_size);

};

#endif
