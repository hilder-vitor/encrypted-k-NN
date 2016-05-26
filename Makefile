DIR_OPE=ope-from-cryptodb/lib/

example: test_neighborhood_several_distributions.cpp $(DIR_OPE)ope.a
	 g++ test_neighborhood_several_distributions.cpp  $(DIR_OPE)ope.a -std=c++11  -lntl -lgmp  -lcrypto -o test_distributions

$(DIR_OPE)ope.a:
	make -C $(DIR_OPE)

clean:
	rm -f test_distributions
	make clean -C $(DIR_OPE)
