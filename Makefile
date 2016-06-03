DIR_OPE=lib/ope/lib/

all: example HomomorphicWeightedKnn.o DataInstance.o EncryptedDataInstance.o Dataset.o main


example: test_neighborhood_several_distributions.cpp $(DIR_OPE)ope.a
	 g++ test_neighborhood_several_distributions.cpp  $(DIR_OPE)ope.a -std=c++11  -lntl -lgmp  -lcrypto -o test_distributions

DataInstance.o: DataInstance.cpp DataInstance.h
	g++ -c DataInstance.cpp -std=c++11  -o DataInstance.o

EncryptedDataInstance.o: EncryptedDataInstance.cpp EncryptedDataInstance.h
	g++ -c EncryptedDataInstance.cpp -std=c++11  -o EncryptedDataInstance.o

HomomorphicWeightedKnn.o:  HomomorphicWeightedKnn.cpp  HomomorphicWeightedKnn.h EncryptedDataInstance.o
	g++ -c  HomomorphicWeightedKnn.cpp -std=c++11  -o  HomomorphicWeightedKnn.o

Dataset.o: Dataset.h Dataset.cpp
	g++ -c  Dataset.cpp -std=c++11  -o  Dataset.o

main: WeightedKnnClient.cpp Dataset.o WeightedKnnClient.h HomomorphicWeightedKnn.o
	g++ WeightedKnnClient.cpp -std=c++11  -o main $(DIR_OPE)ope.a -lntl -lgmp  -lcrypto




$(DIR_OPE)ope.a:
	make -C $(DIR_OPE)

clean:
	rm -f test_distributions main
	rm -f *.o
	make clean -C $(DIR_OPE)
