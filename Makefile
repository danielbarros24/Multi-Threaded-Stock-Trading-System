all: 
	g++ -o main main.cpp loginSystem/loginSystem.cpp loginSystem/sha256.c -g -lsqlite3 -std=c++11