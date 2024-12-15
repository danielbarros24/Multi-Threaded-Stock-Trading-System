#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"
#include <vector>
#include <memory>

namespace TradingSystemClasses {
  class OrderBook {
    public:
          OrderBook();
          void addOrder(const std::shared_ptr<Order>& order); 
          void matchOrders();
          void printOrderBook() const;
          void setStrategy(std::shared_ptr<OrderMatchingStrategy> s);
    private:
          std::vector<std::shared_ptr<Order>> book;
          std::shared_ptr<OrderMatchingStrategy> strategy;
  };
}

#endif //ORDERBOOK_H