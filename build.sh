#!/bin/bash

git submodule update --init
cmake -S external/cpr -B external/cpr
make external/cpr
cd ../..
make .