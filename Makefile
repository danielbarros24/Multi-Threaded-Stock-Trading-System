all: 
	g++ -o main main.cpp src/loginSystem.cpp src/order.cpp src/orderBook.cpp src/sha256.c src/stock.cpp src/trader.cpp -g -lsqlite3 -std=c++14