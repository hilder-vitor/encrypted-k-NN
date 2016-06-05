DIR_OPE=lib/ope/lib/
DIR_YASHE=lib/yashe/src/
DOT_O=DataInstance.o Dataset.o EncryptedDataInstance.o EncryptedDataset.o HomomorphicWeightedKnn.o

all: $(DOT_O) main


example: test_neighborhood_several_distributions.cpp $(DIR_OPE)ope.a
	 g++ test_neighborhood_several_distributions.cpp  $(DIR_OPE)ope.a -std=c++11  -lntl -lgmp  -lcrypto -o test_distributions

DataInstance.o: DataInstance.cpp DataInstance.h
	g++ -c DataInstance.cpp -std=c++11  -o DataInstance.o

EncryptedDataInstance.o: EncryptedDataInstance.cpp EncryptedDataInstance.h
	g++ -c EncryptedDataInstance.cpp -std=c++11  -o EncryptedDataInstance.o

HomomorphicWeightedKnn.o:  HomomorphicWeightedKnn.cpp  HomomorphicWeightedKnn.h EncryptedDataInstance.o
	g++ -c  HomomorphicWeightedKnn.cpp -std=c++11  -o  HomomorphicWeightedKnn.o

Dataset.o: Dataset.h Dataset.cpp DataInstance.o
	g++ -c  Dataset.cpp -std=c++11  -o  Dataset.o

EncryptedDataset.o: EncryptedDataset.h EncryptedDataset.cpp EncryptedDataInstance.o $(DIR_OPE)ope.a
	g++ -c  EncryptedDataset.cpp -std=c++11  -o  EncryptedDataset.o


main: WeightedKnnClient.cpp Dataset.o EncryptedDataset.o HomomorphicWeightedKnn.o yashe.a 
	g++ WeightedKnnClient.cpp $(DOT_O) -std=c++11  -o main $(DIR_OPE)ope.a $(DIR_YASHE)yashe.a -lntl -lgmp -lgmpxx -lcrypto -lm -lmpfr -lflint -pthread -fopenmp

yashe.a: $(DIR_YASHE)Makefile
	make -C $(DIR_YASHE)


$(DIR_OPE)ope.a:
	make -C $(DIR_OPE)

clean:
	rm -f test_distributions main
	rm -f $(DOT_O)
	make clean -C $(DIR_OPE)
