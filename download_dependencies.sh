#!/bin/bash

if [ -d lib ]; then
	rm -rf lib
fi

mkdir lib


###  YASHE LIB
rm -rf lib/yashe
wget https://github.com/hilder-vitor/yashe/archive/master.tar.gz
tar xf master.tar.gz
mkdir -p lib/yashe
mv yashe-master/* lib/yashe/
rmdir yashe-master/
rm master.tar.gz


### OPE LIB
wget https://github.com/hilder-vitor/ope-from-cryptodb/archive/master.tar.gz
tar xf master.tar.gz
mkdir lib/ope
mv ope-from-cryptodb-master/* lib/ope/
rmdir ope-from-cryptodb-master/
rm master.tar.gz

### PAILLIER LIB
wget https://github.com/hilder-vitor/paillier/archive/master.tar.gz
tar xf master.tar.gz
mkdir lib/paillier
mv paillier-master/* lib/paillier
rmdir paillier-master/
rm master.tar.gz

