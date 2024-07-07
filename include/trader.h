#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>
#include "orderBook.h"
#include "stock.h"

namespace TradingSystemClasses {

  class Trader {
  public:
          Trader();
          void buy(const Stock&, int);
          void sell(const Stock&, int);
          void trade(double, double, int, int);
          int getID();
          //const vector<shared_ptr<Order>>& getOrders() const;
          void addStock(Stock);
          std::vector<Stock> getStocks() const;

  private:
          static int id_counter;
          int id;
          std::string name;
          std::vector<Stock> stocks;
          //OrderFactory* orderFactory;
          //OrderBook* orderBook;
          //std::vector<shared_ptr<Order>>&;
  };
}

#endif // TRADER_H