includepath = .
linklib = ./gtest/lib/gtest_main.a

all : test_vector.o test_list.o
	g++ -std=c++17 test_vector.o $(linklib) -lpthread -o test_vector.out
	g++ -std=c++17 test_list.o $(linklib) -lpthread -o test_list.out

debug : test_vector_g.o test_list_g.o
	g++ -std=c++17 test_vector_g.o $(linklib) -lpthread -o test_vector.out
	g++ -std=c++17 test_list_g.o $(linklib) -lpthread -o test_list.out

test_vector_g.o : test_vector.cpp
	g++ -g -c -std=c++17 -o test_vector_g.o -I$(includepath) test_vector.cpp

test_vector.o : test_vector.cpp
	g++ -c -std=c++17 -o test_vector.o -I$(includepath) test_vector.cpp

test_list_g.o : test_list.cpp
	g++ -g -c -std=c++17 -o test_list_g.o -I$(includepath) test_list.cpp

test_list.o : test_list.cpp
	g++ -c -std=c++17 -o test_list.o -I$(includepath) test_list.cpp

clean : 
	rm test_vector.o test_vector_g.o test_list.o test_list_g.o
