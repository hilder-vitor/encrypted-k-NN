#include <vector>
#include <iostream>

#include<NTL/RR.h>

#include "HomomorphicWeightedKnn.h"

using namespace NTL;
using namespace std;

#define FACTOR_TO_INT 10000000000L

void HomomorphicWeightedKnn::set_k(unsigned int neighbourhood_size){
	k = (neighbourhood_size == 0 ? 1 : neighbourhood_size);
}

void HomomorphicWeightedKnn::compute_all_distances(const EncryptedDataInstance& query){
	for (unsigned int i = 0; i < instances.size(); i++){
		instances[i].set_distance(query);
	}
}

void HomomorphicWeightedKnn::sort_by_distance(){
	sort(instances.begin(), instances.end());
}

double inverse_of(ZZ number){
	RR number_RR = MakeRR(number, 0);
	return conv<double>(1.0 / number_RR);
}

mpz_class HomomorphicWeightedKnn::encode_weight(EncryptedDataInstance instance, double total){
	double weight;
	if (instance.get_distance() == 0){
		weight = 0.0000001;
	}else{
		weight = inverse_of(instance.get_distance());
	}
	weight /= total;

	long int w = weight * FACTOR_TO_INT;
	mpz_class encoded_weight(w);
	return encoded_weight;
}

paillier::Ciphertext HomomorphicWeightedKnn::accumulate_classes(){
	double total = sum_of_inverse_distances();
	paillier::Ciphertext class_assigned = paillier.mul(instances[0].get_class(), encode_weight(instances[0], total));
	for (unsigned int i = 1; i < k; i++){
		mpz_class encoded_weight = encode_weight(instances[i], total);
		paillier::Ciphertext enc_encoded_weight = paillier.mul(instances[i].get_class(), encoded_weight);
		class_assigned = paillier.add(class_assigned, enc_encoded_weight);
	}
	return class_assigned;
}

double HomomorphicWeightedKnn::sum_of_inverse_distances(){
	double total = 0.0;
	for (unsigned int i = 0; i < k; i++){
		if (instances[i].get_distance() == 0){
			total += 1.0 / 0.0000001;
		}else{
			total += inverse_of(instances[i].get_distance());
		}
	}
	return total;
}

HomomorphicWeightedKnn::HomomorphicWeightedKnn(unsigned int _k, const vector<EncryptedDataInstance>& _data, paillier::Paillier& public_key) 
	: k(_k), instances(_data), paillier(public_key) {

}


paillier::Ciphertext HomomorphicWeightedKnn::classify(const EncryptedDataInstance& query){
	compute_all_distances(query);
	sort_by_distance();
	return accumulate_classes();
}
