DIR_OPE=lib/ope/lib/
DIR_YASHE=lib/yashe/src/
DIR_PAILLIER=lib/paillier/
DOT_O=DataInstance.o Dataset.o EncryptedDataInstance.o EncryptedDataset.o HomomorphicKnn.o HomomorphicWeightedKnn.o

CXXFLAGS=-I/usr/local/include -I/opt/local/include -I/home/vitor/mylibs/mpfr/include  -I/home/vitor/mylibs/flint/include -I/home/vitor/mylibs/gmp/include -O3 -fopenmp
LDFLAGS=-L/opt/local/lib/  -L/home/vitor/mylibs/flint/lib/ -L/home/vitor/mylibs/gmp/lib/ -L/home/vitor/mylibs/mpfr/lib/ -lntl -lgmp -lgmpxx -lcrypto -lm -lmpfr -lflint 


all: $(DOT_O) knn wknn


DataInstance.o: DataInstance.cpp DataInstance.h
	g++ -c DataInstance.cpp -std=c++11  -o DataInstance.o $(CXXFLAGS) $(LDFLAGS)

EncryptedDataInstance.o: EncryptedDataInstance.cpp EncryptedDataInstance.h
	g++ -c EncryptedDataInstance.cpp -std=c++11  -o EncryptedDataInstance.o $(CXXFLAGS) $(LDFLAGS)

HomomorphicWeightedKnn.o:  HomomorphicWeightedKnn.cpp  HomomorphicWeightedKnn.h EncryptedDataInstance.o
	g++ -c  HomomorphicWeightedKnn.cpp -std=c++11  -o  HomomorphicWeightedKnn.o $(CXXFLAGS) $(LDFLAGS)

HomomorphicKnn.o:  HomomorphicKnn.cpp  HomomorphicKnn.h EncryptedDataset.o
	g++ -c  HomomorphicKnn.cpp -std=c++11  -o  HomomorphicKnn.o $(CXXFLAGS) $(LDFLAGS)


Dataset.o: Dataset.h Dataset.cpp DataInstance.o
	g++ -c  Dataset.cpp -std=c++11  -o  Dataset.o $(CXXFLAGS) $(LDFLAGS)


EncryptedDataset.o: EncryptedDataset.h EncryptedDataset.cpp EncryptedDataInstance.o $(DIR_OPE)ope.a
	g++ -c  EncryptedDataset.cpp -std=c++11  -o  EncryptedDataset.o $(CXXFLAGS) $(LDFLAGS)


wknn: WeightedKnnClient.cpp Dataset.o EncryptedDataset.o HomomorphicWeightedKnn.o yashe.a 
	g++ WeightedKnnClient.cpp $(DOT_O) -std=c++11  -o wknn $(DIR_OPE)ope.a $(DIR_YASHE)yashe.a $(DIR_PAILLIER)src/Paillier.cpp -pthread $(CXXFLAGS) $(LDFLAGS)

knn: KnnClient.cpp Dataset.o EncryptedDataset.o HomomorphicKnn.o yashe.a Paillier.o
	g++ KnnClient.cpp $(DOT_O) -std=c++11  -o knn $(DIR_OPE)ope.a $(DIR_YASHE)yashe.a $(DIR_PAILLIER)src/Paillier.cpp -pthread $(CXXFLAGS) $(LDFLAGS)


yashe.a: $(DIR_YASHE)Makefile
	make -C $(DIR_YASHE)

Paillier.o: $(DIR_PAILLIER)Makefile
	make -C $(DIR_PAILLIER)

$(DIR_OPE)ope.a:
	make -C $(DIR_OPE)

clean:
	rm -f $(DOT_O) knn wknn
	make clean -C $(DIR_OPE)
