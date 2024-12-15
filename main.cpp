#include "include/loginSystem.h"
#include "include/orderBook.h"
#include "include/trader.h"

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>

using namespace std;
using namespace TradingSystemClasses;

void traderFunction(Trader& trader, OrderBook& orderBook) {
    Stock stockone;
		stockone.setPrice(120);
		stockone.setName("one");

    Stock stocktwo;
		stocktwo.setPrice(160);
		stocktwo.setName("two");

    trader.addStock(stockone);
    trader.addStock(stocktwo);
    int quantity = 30;

    // Set Buy and Sell Orders
    std::shared_ptr<Order> buyOrder = std::make_shared<MarketOrder>(trader.getID(), stockone.getPrice(), quantity);
    buyOrder->setOrderType(OrderType::BUY);
    std::shared_ptr<Order> sellOrder = std::make_shared<MarketOrder>(trader.getID(), stocktwo.getPrice(), quantity);
    sellOrder->setOrderType(OrderType::SELL);

    // Simulates Trading Process
    trader.trade(100, 120, 30, 50);

    // Add Orders to OrderBook
    orderBook.addOrder(buyOrder);
    orderBook.addOrder(sellOrder);
}

int main() {
	if (loginSystem()){
		cout << "ENTERING IN APPLICATION..." << endl;
		
		// Create the order book to store orders
		std::shared_ptr<OrderBook> orderBook = std::make_shared<OrderBook>();

		// Create and start multiple trader threads (simulating multiple traders)
		vector<thread> traderThreads;
		int numTraders = 5;  // Simulate 5 traders
		for (int i = 0; i < numTraders; ++i) {
				Trader trader("Trader " + to_string(i + 1));  // Create a new trader with a unique name
				traderThreads.emplace_back([&]() {traderFunction(trader, *orderBook);});  // Launch a new thread for each trader
		}

		// Wait for all trader threads to finish their work
		for (auto& thread : traderThreads) {
			thread.join();  // Join each thread back to the main thread
		}

		// Print the order book after all trading actions are completed
		orderBook->printOrderBook();
	}

	return 0;   
}