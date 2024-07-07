#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <memory>
#include <ctime>

namespace TradingSystemClasses {

  enum OrderType {
    BUY,
    SELL
  };

  class Order {
    public:
          Order();
          std::string getOrderType();
          int getTraiderId();
          double getPrice();
          int getQuantity();
          time_t getTimestamp();
          void reduceQuantity();
          OrderType getOrderType() const;
          void setOrderType(OrderType);
    private:
          int trader_id;
          double price;
          int quantity;
          std::time_t timestamp;
          OrderType ordertype;
  };
}


#endif //ORDER_H