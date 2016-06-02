#ifndef __HEKNN_DATA_INSTANCE_
#define __HEKNN_DATA_INSTANCE_

#include <vector>
#include <iostream>

class DataInstance {
    private:
	std::vector<int> data;
	int id;
	int label; // the class assigned to this data_instance
	int distance; // the distance to the query vector
    
    public:

	DataInstance(int id, const std::vector<int>& data, int _class);

	void set_distance(const DataInstance& query);
	int get_distance() const;

	int get_id() const;
	int get_class() const;

	int size() const; // the size of the data vector

	bool operator<(const DataInstance&) const;
	
	const int& operator[](const int&) const;
};	

std::ostream& operator<<(std::ostream&, const DataInstance&);
#endif
