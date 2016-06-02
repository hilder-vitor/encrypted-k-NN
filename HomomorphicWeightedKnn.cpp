#include <vector>
#include <iostream>

#include<NTL/RR.h>

#include "HomomorphicWeightedKnn.h"
#include "lib/yashe/src/Yashe.h"
#include "lib/yashe/src/CoefficientwiseCRT.h"

using namespace NTL;

void HomomorphicWeightedKnn::compute_all_distances(const EncryptedDataInstance& query){
	for (unsigned int i = 0; i < instances.size(); i++){
		instances[i].set_distance(query);
	}
}

void HomomorphicWeightedKnn::sort_by_distance(){
	//function <bool()> my_comparison = [](const EncryptedDataInstance& a, const EncryptedDataInstance& b){ return a < b};
	//sort(instances.begin(), instances.end(), my_comparison);
	sort(instances.begin(), instances.end());
}

double HomomorphicWeightedKnn::sum_of_inverse_distances(){
	RR total;
	total = 0;
	for (unsigned int i = 0; i < k; i++){
		RR inv_dist = MakeRR(instances[i].get_distance(), 0);
		inv_dist = 1.0 / inv_dist;
		total += inv_dist;
	}
	return conv<double>(total);
}

HomomorphicWeightedKnn::HomomorphicWeightedKnn(unsigned int _k, const vector<EncryptedDataInstance>& _data, const Yashe& public_key, const CoefficientwiseCRT& _crt) 
	: k(_k), instances(_data), pk(public_key), crt(_crt) {
	
}


RealNumberCiphertext HomomorphicWeightedKnn::classify(const EncryptedDataInstance& query){
	compute_all_distances(query);
	sort_by_distance();
	double total = sum_of_inverse_distances();

	/* XXX: acumulate the distances */
}
