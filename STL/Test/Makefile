includepath = .
linklib = ./gtest/lib/gtest_main.a

all : test_vector.o
	g++ -std=c++17 test_vector.o $(linklib) -std=c++11 -lpthread -o test_vector.out

debug : test_vector_g.o
	g++ -std=c++17 test_vector_g.o $(linklib) -std=c++11 -lpthread -o test_vector.out

test_vector_g.o : test_vector.cpp
	g++ -g -c -std=c++17 -o test_vector_g.o -I$(includepath) test_vector.cpp

test_vector.o : test_vector.cpp
	g++ -c -std=c++17 -o test_vector.o -I$(includepath) test_vector.cpp

clean : 
	rm test_vector.o
