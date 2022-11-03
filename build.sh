#!/bin/bash

git submodule update --init
cmake -S external/cpr -B external/cpr
cd external/cpr
make .
cd -
make cppcouchconnector