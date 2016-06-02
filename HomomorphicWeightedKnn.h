#ifndef __HEKNN__CLOUD_
#define __HEKNN__CLOUD_

#include <vector>
#include <iostream>

#include "EncryptedDataInstance.h"
#include "lib/yashe/src/Yashe.h"
#include "lib/yashe/src/CoefficientwiseCRT.h"


class HomomorphicWeightedKnn {
    private:
	unsigned int k;
	vector<EncryptedDataInstance> instances;
	const Yashe& pk;
	const CoefficientwiseCRT& crt;

	// auxiliar functions
	void compute_all_distances(const EncryptedDataInstance& query);
	void sort_by_distance();
	double sum_of_inverse_distances();
	
    
    public:

	HomomorphicWeightedKnn(unsigned int _k, const vector<EncryptedDataInstance>& _data, const Yashe& pk, const CoefficientwiseCRT& crt);

	RealNumberCiphertext classify(const EncryptedDataInstance& query);

	void set_k(unsigned int neighbourhood_size);

};

#endif
