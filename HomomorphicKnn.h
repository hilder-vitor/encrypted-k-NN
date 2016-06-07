#ifndef __HEKNN__CLOUD_
#define __HEKNN__CLOUD_

#include <vector>
#include <iostream>

#include "EncryptedDataInstance.h"
#include "lib/yashe/src/Yashe.h"


class HomomorphicKnn {
    private:
	unsigned int k;
	vector<EncryptedDataInstance> instances;
	Yashe& yashe;

	// auxiliar functions
	void compute_all_distances(const EncryptedDataInstance& query);
	void sort_by_distance();
	double sum_of_inverse_distances();
	RealNumberCiphertext accumulate_classes();

    public:

	HomomorphicKnn(unsigned int _k, const vector<EncryptedDataInstance>& _data, Yashe& pk);

	RealNumberCiphertext classify(const EncryptedDataInstance& query);

	void set_k(unsigned int neighbourhood_size);

};

#endif
