#include "DataInstance.h"

using namespace std;

DataInstance::DataInstance(int _id, const std::vector<int>& _data, int _class) : id(_id), data(_data), label(_class), distance(-1){
}

void DataInstance::set_distance(const DataInstance& query){
	distance = 0;
	unsigned int P = data.size();
	for (unsigned int i = 0; i < P; i++){
		distance += (query[i] - data[i]) * (query[i] - data[i]);
	}
}

int DataInstance::get_distance() const {
	return distance;
}

int DataInstance::get_id() const{
	return id;
}

int DataInstance::get_class() const{
	return label;
}

int DataInstance::size() const{
	return data.size();
}

bool DataInstance::operator<(const DataInstance& other) const {
	return distance < other.get_distance();
}
	

const int& DataInstance::operator[](const int& i) const{
	return data[i];
}

std::ostream& operator<<(std::ostream& os, const DataInstance& v){
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
