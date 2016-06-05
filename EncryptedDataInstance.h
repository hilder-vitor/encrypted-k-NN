#ifndef __HEKNN_ENCRYPTED_DATA_INSTANCE_
#define __HEKNN_ENCRYPTED_DATA_INSTANCE_

#include <vector>
#include <iostream>

#include "lib/ope/lib/ope.hh"
#include "lib/yashe/src/Yashe.h"


class EncryptedDataInstance {
    private:
	std::vector<NTL::ZZ> data;
	int id;
	RealNumberCiphertext label; // the class assigned to this data_instance
	NTL::ZZ distance; // the distance to the query vector
    
    public:

	EncryptedDataInstance(int id, const std::vector<NTL::ZZ>& data, const RealNumberCiphertext& _class);

	void set_distance(const EncryptedDataInstance& query);
	NTL::ZZ get_distance() const;

	int get_id() const;
	RealNumberCiphertext get_class() const;

	int size() const;

	bool operator<(const EncryptedDataInstance&) const;
	
	const NTL::ZZ& operator[](const int&) const;
};	

std::ostream& operator<<(std::ostream&, const EncryptedDataInstance&);
#endif
