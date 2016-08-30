#include <vector>
#include <iostream>

#include<NTL/RR.h>

#include "HomomorphicKnn.h"

using namespace NTL;
using namespace std;


void HomomorphicKnn::set_k(unsigned int neighbourhood_size){
	k = (neighbourhood_size == 0 ? 1 : neighbourhood_size);
}

void HomomorphicKnn::compute_all_distances(const EncryptedDataInstance& query){
	for (unsigned int i = 0; i < instances.size(); i++){
		instances[i].set_distance(query);
	}
}

void HomomorphicKnn::sort_by_distance(){
	sort(instances.begin(), instances.end());
}

vector<mpz_class> HomomorphicKnn::accumulate_classes(){
	vector<mpz_class> class_assigned = instances[0].get_class();
	for (unsigned int i = 1; i < k; i++){
		//class_assigned += instances[i].get_class();
		class_assigned = paillier.add(class_assigned, instances[i].get_class());
	}
	return class_assigned;
}

HomomorphicKnn::HomomorphicKnn(unsigned int _k, const vector<EncryptedDataInstance>& _data, Paillier& public_key) 
	: k(_k), instances(_data), paillier(public_key) {
	
}

vector<mpz_class> HomomorphicKnn::classify(const EncryptedDataInstance& query){
	compute_all_distances(query);
	sort_by_distance();
	return accumulate_classes();
}
