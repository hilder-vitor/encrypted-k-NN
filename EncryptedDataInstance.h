#ifndef __HEKNN_ENCRYPTED_DATA_INSTANCE_
#define __HEKNN_ENCRYPTED_DATA_INSTANCE_

#include <vector>
#include <iostream>

#include "lib/ope/lib/ope.hh"
#include "lib/paillier/src/Paillier.h"


class EncryptedDataInstance {
    private:
	std::vector<long int> data;
	int id;
	paillier::Ciphertext label; // the class assigned to this data_instance
	double distance; // the distance to the query vector
    
    public:

	EncryptedDataInstance(int id, const std::vector<NTL::ZZ>& data, const paillier::Ciphertext& _class);

	void set_distance(const EncryptedDataInstance& query);
	double get_distance() const;

	int get_id() const;
	paillier::Ciphertext get_class() const;

	int size() const;

	bool operator<(const EncryptedDataInstance&) const;
	
	const long int& operator[](const int&) const;
};	

std::ostream& operator<<(std::ostream&, const EncryptedDataInstance&);
#endif
