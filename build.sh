#!/bin/bash

git submodule update --init
cd external/cpr
cmake
make .
cd ../..
make .