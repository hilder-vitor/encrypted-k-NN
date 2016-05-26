#include "ope-from-cryptodb/lib/ope.hh"
#include <string>
#include <NTL/ZZ.h>
#include <NTL/RR.h>

#include <cstdlib>

#include <random>
#include <functional>
#include <algorithm>

using namespace std;
using namespace NTL;

typedef struct __distance {
	unsigned int name;
	ZZ dist;
} DISTANCE;


typedef struct __vec {
	unsigned int name;
	vector<int> data;
} VEC;

typedef struct __enc_vec {
	unsigned int name;
	vector<ZZ> data;
} ENC_VEC;

std::ostream& operator<<(std::ostream& os, const DISTANCE& d) {
	cout << "<" << d.name << ", " << d.dist << "> ";
}

std::ostream& operator<<(std::ostream& os, const VEC& v) {
	os << v.name << " --> [";
	unsigned int P = v.data.size();
	if (0 == P){
		os << "]" << endl;
	}else{
		for (unsigned int i = 0; i < P - 1; i++){
			os << v.data[i] << ", ";
		}
		os << v.data[P - 1] << "]" << endl;
	}
	return os;
}
std::ostream& operator<<(std::ostream& os, const ENC_VEC& v) {
	os << v.name << " --> [";
	unsigned int P = v.data.size();
	if (0 == P){
		os << "]" << endl;
	}else{
		for (unsigned int i = 0; i < P - 1; i++){
			os << v.data[i] << ", ";
		}
		os << v.data[P - 1] << "]" << endl;
	}
	return os;
}

template <typename T> 
std::ostream& operator<<(std::ostream& os, const vector<T>& v) {
	unsigned int N = v.size();
	for (unsigned int i = 0; i < N; i++){
		cout << v[i] << " ";
	}
	cout << endl;
}

vector<VEC> random_VEC(unsigned int number_of_vectors, unsigned int dimension, function <int()> rand_gen){
	vector<VEC> v(number_of_vectors);
	for (unsigned int i = 0; i < number_of_vectors; i++){
		v[i].name = i;
		for (unsigned int j = 0; j <  dimension; j++){
			v[i].data.push_back(rand_gen());
		}
	}
	return v;
}

template <typename T> 
void printVector(vector<T> u){
	unsigned int n = u.size();
	for (unsigned int i = 0; i < n; i++){
		cout << u[i] << " ";
	}
	cout << endl;
}

vector<ZZ> encryptVector(OPE& o, vector<int> plaintexts){
	unsigned int n = plaintexts.size();
	vector<ZZ> ciphers(n);
	for (unsigned int i = 0; i < n; i++){
		ciphers[i] = o.encrypt(to_ZZ(plaintexts[i]));
	}
	return ciphers;
}

vector<ENC_VEC> encryptVEC (OPE& o, vector<VEC> plaintexts){
	unsigned int n = plaintexts.size();
	vector<ENC_VEC> ciphers(n);
	for (unsigned int i = 0; i < n; i++){
		ciphers[i].name = i;
		ciphers[i].data = encryptVector(o, plaintexts[i].data);
	}
	return ciphers;
}

template <typename T> 
DISTANCE compute_distance(T vec, T query){
	DISTANCE d;
	d.name = vec.name;
	d.dist = to_ZZ(0);
	unsigned int P = query.data.size();
	for (unsigned int i = 0; i < P; i++){
		d.dist += (query.data[i] - vec.data[i]) * (query.data[i] - vec.data[i]);
	}
	return d;
}

template <typename T> 
vector<DISTANCE> compute_distances(vector<T> vecs, T query){
	unsigned int N = vecs.size();
	vector<DISTANCE> distances(N);
	for (unsigned int i = 0; i < N; i++){
		distances[i] = compute_distance(vecs[i], query);
	}
	return distances;
}


/*
void teste_vizinhancas(OPE& o, unsigned int number_of_vectors){
	unsigned int N = number_of_vectors;
	vector<VEC> vecs = random_VEC(N, 2);
	vector<ENC_VEC> enc_vecs = encryptVEC(o, vecs);
	for (unsigned int i = 0; i < N; i++){
		cout << vecs[i] << endl;
		cout << enc_vecs[i] << endl;
		cout << endl;
	}
}
*/

bool distance_comparison(DISTANCE d1, DISTANCE d2){
	return d1.dist < d2.dist;
}

void sort_distances(vector<DISTANCE>& distances){
	sort(distances.begin(), distances.end(), distance_comparison);
}

/**
 *	Test if the k first vectors in dist1 are among the k first vectors of dist2 and returning the percentual 
 * of intersection.
 * 		For instance:
 *		 	If the k vectors of dist1 are in dist2, it returns 1.0  (100%)
 *		 	If any k/2 vectors of dist1 are in dist2, it returns 0.5  (50%)
 *		 	If none of the k vectors of dist1 are in dist2, it returns 0.0  (0%)
 */
double k_neighborhood_intersection(const vector<DISTANCE>& dist1, const vector<DISTANCE>& dist2, unsigned int k){
	double percentual = 0.0;
	for (unsigned int i = 0; i < k; i++){
		bool found = false;
		for (unsigned int j = 0; j < k && !found; j++){
			if (dist1[i].name == dist2[j].name)
				found = true;
		}
		// if vec dist1[i].name isn't among the k first vecs of dist2
		if (found){
			percentual++;
		}
	}
	return percentual / k;;
}

double test_distribution(OPE& o, unsigned int N, unsigned int P, unsigned int k, function <int()> my_rand, unsigned int& ok, unsigned int& not_ok){
	ok = not_ok = 0;
	unsigned int number_of_tests = 10;
	vector<VEC> instances = random_VEC(N, P, my_rand);
	vector<VEC> queries = random_VEC(number_of_tests, P, my_rand);

	vector<ENC_VEC> enc_instances = encryptVEC (o, instances);
	vector<ENC_VEC> enc_queries = encryptVEC (o, queries);

	double avg_not_ok = 0.0;
	for (unsigned int i = 0; i < number_of_tests; i++){
		vector<DISTANCE> original_distances =  compute_distances(instances, queries[i]);
		vector<DISTANCE> enc_distances =  compute_distances(enc_instances, enc_queries[i]);
		sort_distances(original_distances);
		sort_distances(enc_distances);

		double percent = k_neighborhood_intersection(original_distances, enc_distances, k);
		if (1 == percent)
			ok++;
		else{
			not_ok++;
			avg_not_ok += percent;
		}
	}
	if (not_ok)
		return avg_not_ok / not_ok;
	return -1;
}

void test_normal_distribution(OPE& o){
	unsigned int N = 100;
	unsigned int P = 10;
	unsigned int k = 3;
	double gaussian_avg= 100.0;
	double gaussian_std = 25.0;

	std::default_random_engine generator(time(0));

	double percent;
	unsigned int ok;
	unsigned int not_ok;
	for (; N < 1100; N += 100){
		for (P = 10; P < N/2 && P <= 100; P += 5){
			for (gaussian_avg = 100.0; gaussian_avg <= 1000; gaussian_avg += 100){
				for (gaussian_std = 25.0; gaussian_std <= gaussian_avg / 2; gaussian_std *= 2){
					for (k = 1; k < 9; k++){
						std::normal_distribution<double> distribution(gaussian_avg, gaussian_std);
						function <int()> my_gaussian_rand = [&generator, &distribution](){ return distribution(generator);};
						double avg_not_ok = test_distribution(o, N, P, k, my_gaussian_rand, ok, not_ok);
						cout << N << "," << P << "," << gaussian_avg << "," << gaussian_std << "," << k << "," << ok << "," << not_ok << "," << avg_not_ok << endl;
					}
				}
			}
		}
	}
}

int
main(int argc, char **argv)
{
	if (argc < 2){
		cout << "ERROR: usage\n   " << argv[0] << " <dimension> <P> <C>" << endl;
		cout << "    <P>: the length of plaintext range (plaintexts are in [0, 2**P - 1])" << endl;
		cout << "    <C>: the length of ciphertext range (ciphertexts are in [0, 2**C - 1])" << endl;
		return 0;
	}

	// plaintext range's length in bits (plaintexts are in [0, 2**P[
	unsigned int P = (unsigned int) stoi(argv[1]);
	// ciphertext range's length in bits (ciphertexts are in [0, 2**C[
	unsigned int C = (unsigned int) stoi(argv[2]);

	OPE o("My v3Ry $TR0NG Key", P, C);

	test_normal_distribution(o);
	return 0;
}
