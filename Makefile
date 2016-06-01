DIR_OPE=lib/ope/lib/

all: example DataInstance.o EncryptedDataInstance.o


example: test_neighborhood_several_distributions.cpp $(DIR_OPE)ope.a
	 g++ test_neighborhood_several_distributions.cpp  $(DIR_OPE)ope.a -std=c++11  -lntl -lgmp  -lcrypto -o test_distributions

DataInstance.o: DataInstance.cpp DataInstance.h
	g++ -c DataInstance.cpp -std=c++11  -o DataInstance.o

EncryptedDataInstance.o: EncryptedDataInstance.cpp EncryptedDataInstance.h
	g++ -c EncryptedDataInstance.cpp -std=c++11  -o EncryptedDataInstance.o



$(DIR_OPE)ope.a:
	make -C $(DIR_OPE)

clean:
	rm -f test_distributions
	rm -f *.o
	make clean -C $(DIR_OPE)
