#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>
#include <memory>
#include "orderBook.h"
#include "stock.h"

namespace TradingSystemClasses {

  class TraderException : public std::exception
  {
  public:
          TraderException(const std::string&);
          virtual const char* what() const noexcept override;
  private:
          std::string message;
  };

  class Trader {
  public:
          Trader();
          explicit Trader(const std::string& n);
          Trader(const std::string& n, const std::vector<Stock>& s, std::shared_ptr<OrderFactory> o, std::shared_ptr<OrderBook> b);
          void buy(const Stock&, int quantity);
          void sell(const Stock&, int quantity);
          void trade(double buyThreshold, double sellThreshold, int buyQuantity, int sellQuantity);
          int getID() const;
          const std::vector<std::shared_ptr<Order>>& getOrders() const;
          void addStock(const Stock& st);
          std::vector<Stock> getStocks() const;
          ~Trader();

  private:
          static int id_counter;
          int id;
          std::string name;
          std::vector<Stock> stocks;
          std::vector<std::shared_ptr<Order>> orders;
          std::shared_ptr<OrderFactory> orderFactory;
          std::shared_ptr<OrderBook> orderBook;  
  };
}

#endif // TRADER_H