#include <vector>
#include <iostream>

#include<NTL/RR.h>

#include "HomomorphicWeightedKnn.h"
#include "lib/yashe/src/Yashe.h"
#include "lib/yashe/src/CoefficientwiseCRT.h"

using namespace NTL;


void HomomorphicWeightedKnn::set_k(unsigned int neighbourhood_size){
	k = (neighbourhood_size == 0 ? 1 : neighbourhood_size);
}

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

RealNumberCiphertext HomomorphicWeightedKnn::accumulate_classes(){
	double total = sum_of_inverse_distances();
	RealNumberCiphertext class_assigned = yashe.encrypt(yashe.encode(0.0));
	for (unsigned int i = 0; i < k; i++){
		double weight = inverse_of(instances[i].get_distance());
		weight /= total;
		// XXX: no need for CRT because coefficients are 0 or 1
		RealNumberPlaintext plain_weight = yashe.encode(weight);
		class_assigned += plain_weight * instances[i].get_class();		
	}
	return class_assigned;
}

double inverse_of(ZZ number){
	RR number_RR = MakeRR(number_RR, 0);
	return conv<double>(1.0 / number_RR);
}

double HomomorphicWeightedKnn::sum_of_inverse_distances(){
	double total = 0.0;
	for (unsigned int i = 0; i < k; i++){
		total += inverse_of(instances[i].get_distance());
	}
	return total;
}

HomomorphicWeightedKnn::HomomorphicWeightedKnn(unsigned int _k, const vector<EncryptedDataInstance>& _data, const Yashe& public_key, const CoefficientwiseCRT& _crt) 
	: k(_k), instances(_data), yashe(public_key), crt(_crt) {
	
}


RealNumberCiphertext HomomorphicWeightedKnn::classify(const EncryptedDataInstance& query){
	compute_all_distances(query);
	sort_by_distance();
	return accumulate_classes();
}
