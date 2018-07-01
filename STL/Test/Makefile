includepath = .
linklib = ./gtest/lib/gtest_main.a

all : test_vector.o
	g++ -std=c++17 test_vector.o $(linklib) -std=c++11 -lpthread -o test_vector

test_vector.o : test_vector.cpp
	g++ -c -std=c++17 -o test_vector.o -I$(includepath) test_vector.cpp

clean : 
	rm test_vector.o
