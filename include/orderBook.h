#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <vector>
#include <memory>
#include "order.h"

namespace TradingSystemClasses {
  class OrderBook {
    public:
          OrderBook();
          void addOrder(std::vector<shared_ptr<Order>>);
          void matchOrders();
          void printOrderBook();
          //void setStrategy(OrderMatchingStrategy);
    private:
          std::vector<shared_ptr<Order>> book;
          //OrderMatchingStrategy strategy;
  };
}


#endif //ORDERBOOK_H