#include "../include/trader.h"
#include <iostream>
#include <memory>

using namespace TradingSystemClasses;

TraderException::TraderException(const std::string& mess) 
      : message(mess) 
{
}

const char* TraderException::what() const noexcept
{ 
      return message.c_str(); 
}

// Static Member Initialization
int Trader::id_counter = 0;

// Trader Constructors
Trader::Trader()
      : name{""}
      , id{id_counter++}
{
}

Trader::Trader(const std::string& n)
      : name{n}
      , id{id_counter++}
{
}

Trader::Trader(const std::string& n, const std::vector<Stock>& s, std::shared_ptr<OrderFactory> o, std::shared_ptr<OrderBook> b)
      : id{id_counter++}
      , name{n}
      , stocks{s}
      , orderFactory{std::move(o)}
      , orderBook{std::move(b)}
{
}

// Buy and Sell methods
void Trader::buy(const TradingSystemClasses::Stock& stock, int quantity) {
  try {
    if (orderFactory && orderBook) {
      auto ord = orderFactory->createOrder(id, stock.getPrice(), quantity);
      if (ord) {
        orders.push_back(std::move(ord));
        orderBook->addOrder(std::move(ord));
      }
    }
  } catch (const TraderException& e) {
    std::cerr << "Trader Exception: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

void Trader::sell(const TradingSystemClasses::Stock& stock, int quantity) {
  try {
    if (orderFactory && orderBook) {
      auto ord = orderFactory->createOrder(id, -stock.getPrice(), quantity);
      if (ord) {
        orders.push_back(std::move(ord));
        orderBook->addOrder(std::move(ord));
      }
    }
  } catch (const TraderException& e) {
    std::cerr << "Trader Exception: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

// Trade Logic
void Trader::trade(double buyThreshold, double sellThreshold, int buyQuantity, int sellQuantity) {
  for (const Stock& stock: stocks) {
    if (stock.getPrice() < buyThreshold) {
      buy(stock, buyQuantity);
    } else if (stock.getPrice() > sellThreshold) {
      sell(stock, sellQuantity);
    }
  }
}

// Accessors and Mutators
int Trader::getID() const {
  return id;
}

const std::vector<std::shared_ptr<Order>>& Trader::getOrders() const {
  return orders;
}

void Trader::addStock(const Stock& st) {
  stocks.push_back(st);
}

std::vector<Stock> Trader::getStocks() const {
  return stocks;
}

//Destructor
Trader::~Trader() = default;