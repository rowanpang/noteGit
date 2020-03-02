#!/bin/bash

rm -rf ./my-build*

mkdir my-build
cd my-build

cmake ../
make
echo '---------------'
./demo
cd -

echo
echo
echo '###############'
mkdir my-build-2
cd my-build-2
cmake ../ -DFOO_ENABLE=0
make
echo '---------------'
./demo
cd -
