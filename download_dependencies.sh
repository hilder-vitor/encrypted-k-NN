#!/bin/bash

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
mv ope-from-cryptodb-master/ lib/ope/
rm master.tar.gz
