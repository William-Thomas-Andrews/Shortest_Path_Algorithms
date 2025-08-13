#! /bin/sh

cd ../src/data/data_generator ; g++ -std=c++20 data_generator.cpp -o ../data_set/data_gen.o; cd ../data_set ; ./data_gen.o