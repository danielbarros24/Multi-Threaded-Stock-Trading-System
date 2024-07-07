all: 
	g++ -o main main.cpp src/loginSystem.cpp src/sha256.c src/trader.cpp -g -lsqlite3 -std=c++11