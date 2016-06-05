#include "EncryptedDataInstance.h"

using namespace std;
using namespace NTL;

EncryptedDataInstance::EncryptedDataInstance(int _id, const std::vector<ZZ>& _data, const RealNumberCiphertext& _class) : id(_id), data(_data), label(_class), distance(to_ZZ(-1)){
}

void EncryptedDataInstance::set_distance(const EncryptedDataInstance& query){
	distance = 0;
	unsigned int P = data.size();
	for (unsigned int i = 0; i < P; i++){
		distance += (query[i] - data[i]) * (query[i] - data[i]);
	}
}

ZZ EncryptedDataInstance::get_distance() const {
	return distance;
}

int EncryptedDataInstance::get_id() const{
	return id;
}

RealNumberCiphertext EncryptedDataInstance::get_class() const{
	return label;
}

int EncryptedDataInstance::size() const{
	return data.size();
}

bool EncryptedDataInstance::operator<(const EncryptedDataInstance& other) const {
	return distance < other.get_distance();
}
	

const ZZ& EncryptedDataInstance::operator[](const int& i) const{
	return data[i];
}


std::ostream& operator<<(std::ostream& os, const EncryptedDataInstance& v){
	os << v.get_id() << " --> [";
	unsigned int P = v.size();
	if (0 == P){
		os << "]";
	}else{
		for (unsigned int i = 0; i < P - 1; i++){
			os << v[i] << ", ";
		}
		os << v[P - 1] << "]";
	}
	os << " class: " << v.get_class() << endl;
	return os;
}
