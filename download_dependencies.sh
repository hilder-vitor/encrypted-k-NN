#!/bin/bash

###  YASHE LIB
rm -rf lib/yashe
wget https://github.com/hilder-vitor/yashe/archive/master.zip
unzip master.zip
mkdir -p lib/yashe
mv yashe-master/* lib/yashe/
rmdir yashe-master/
rm master.zip


### OPE LIB
wget https://github.com/hilder-vitor/ope-from-cryptodb/archive/master.zip
unzip master.zip
mv ope-from-cryptodb-master/ lib/ope/
rm master.zip

