#include "../include/orderBook.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace TradingSystemClasses;
using namespace std;


// Constructor
OrderBook::OrderBook() : strategy(nullptr) 
{
}

// Add Orders
void OrderBook::addOrder(const std::shared_ptr<Order>& order) {
    book.push_back(order);
}

// Match Orders
void OrderBook::matchOrders() {
  std::vector<std::shared_ptr<Order>> buyOrders;
  std::vector<std::shared_ptr<Order>> sellOrders;

  for (const auto& order : book) {
    if (order->getPrice() > 0) {
      buyOrders.push_back(order);
    } else {
      sellOrders.push_back(order);
    }
  }

  if (strategy) {
    strategy->matchOrders(buyOrders, sellOrders);
  } else {
    std::cerr << "Error: No matching strategy set for OrderBook.\n";
  }

  //update book of orders
  book.clear();
  book.insert(book.end(), buyOrders.begin(), buyOrders.end()); 
  book.insert(book.end(), sellOrders.begin(), sellOrders.end()); 
}

// Print Book Orders
void OrderBook::printOrderBook() const {
  if (book.empty()) {
    std::cout << "Order Book is empty.\n";
    return;
  }

  std::cout << "Order Book content...\n";
  std::cout << "ID\tPrice\tQuantity\tOrder Type\n";

  for (const auto& order : book) {
    std::string orderTypeName = (order->getOrdertype() == OrderType::BUY) ? "Buy" : "Sell";
    std::cout << order->getTraderId() << "\t" << order->getPrice() << "\t" << order->getQuantity() << "\t\t" << orderTypeName << "\n";  // Corrigido para getTraderId
  }
  std::cout << "\n";
}

//Configure Strategy
void OrderBook::setStrategy(std::shared_ptr<OrderMatchingStrategy> s) {
  strategy = std::move(s); 
}