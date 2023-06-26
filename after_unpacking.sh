#!/bin/bash

P=ImageDSP/build/linux/
B=$P/build-ImageDSP-Desktop-Debug
qmake $P/ImageDSP/ImageDSP.pro -o $B/Makefile \
    -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
make -C $B -j6
