DIR_OPE=lib/ope/lib/
DIR_YASHE=lib/yashe/src/
DIR_PAILLIER=lib/paillier/
DOT_O=DataInstance.o Dataset.o EncryptedDataInstance.o EncryptedDataset.o EncryptedDataset_Unweighted.o HomomorphicKnn.o

all: $(DOT_O) knn 
	#wknn


example: test_neighborhood_several_distributions.cpp $(DIR_OPE)ope.a
	 g++ test_neighborhood_several_distributions.cpp  $(DIR_OPE)ope.a -std=c++11  -lntl -lgmp  -lcrypto -o test_distributions

DataInstance.o: DataInstance.cpp DataInstance.h
	g++ -c DataInstance.cpp -std=c++11  -o DataInstance.o

EncryptedDataInstance.o: EncryptedDataInstance.cpp EncryptedDataInstance.h
	g++ -c EncryptedDataInstance.cpp -std=c++11  -o EncryptedDataInstance.o

#HomomorphicWeightedKnn.o:  HomomorphicWeightedKnn.cpp  HomomorphicWeightedKnn.h EncryptedDataInstance.o
#	g++ -c  HomomorphicWeightedKnn.cpp -std=c++11  -o  HomomorphicWeightedKnn.o

HomomorphicKnn.o:  HomomorphicKnn.cpp  HomomorphicKnn.h EncryptedDataset_Unweighted.o
	g++ -c  HomomorphicKnn.cpp -std=c++11  -o  HomomorphicKnn.o


Dataset.o: Dataset.h Dataset.cpp DataInstance.o
	g++ -c  Dataset.cpp -std=c++11  -o  Dataset.o

EncryptedDataset.o: EncryptedDataset.h EncryptedDataset.cpp EncryptedDataInstance.o $(DIR_OPE)ope.a
	g++ -c  EncryptedDataset.cpp -std=c++11  -o  EncryptedDataset.o

EncryptedDataset_Unweighted.o: EncryptedDataset_Unweighted.h EncryptedDataset_Unweighted.cpp EncryptedDataInstance.o $(DIR_OPE)ope.a
	g++ -c  EncryptedDataset_Unweighted.cpp -std=c++11  -o  EncryptedDataset_Unweighted.o


#wknn: WeightedKnnClient.cpp Dataset.o EncryptedDataset.o HomomorphicWeightedKnn.o yashe.a 
#	g++ WeightedKnnClient.cpp $(DOT_O) -std=c++11  -o wknn $(DIR_OPE)ope.a $(DIR_YASHE)yashe.a -lntl -lgmp -lgmpxx -lcrypto -lm -lmpfr -lflint -pthread -fopenmp

knn: KnnClient.cpp Dataset.o EncryptedDataset_Unweighted.o HomomorphicKnn.o yashe.a Paillier.o
	g++ KnnClient.cpp $(DOT_O) -std=c++11  -o knn $(DIR_OPE)ope.a $(DIR_YASHE)yashe.a $(DIR_PAILLIER)src/Paillier.cpp -lntl -lgmp -lgmpxx -lcrypto -lm -lmpfr -lflint -pthread -fopenmp


yashe.a: $(DIR_YASHE)Makefile
	make -C $(DIR_YASHE)

Paillier.o: $(DIR_PAILLIER)Makefile
	make -C $(DIR_PAILLIER)

$(DIR_OPE)ope.a:
	make -C $(DIR_OPE)

clean:
	rm -f test_distributions main
	rm -f $(DOT_O)
	make clean -C $(DIR_OPE)
