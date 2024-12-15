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
          Order(int traderId, double price, int quantity, OrderType type);
          virtual ~Order();
          int getTraderId() const;
          double getPrice() const;
          int getQuantity() const;
          std::time_t getTimestamp() const;
          void reduceQuantity(int amount);
          virtual std::string getOrderType() const = 0;
          OrderType getOrdertype() const;
          void setOrderType(OrderType type);

    private:
          int trader_id;
          double price;
          int quantity;
          std::time_t timestamp;
          OrderType ordertype;
  };

  class MarketOrder : public Order {
    public:
      MarketOrder(int trader_id, double price, int quantity) : Order(trader_id, price, quantity, OrderType::BUY) {}
      std::string getOrderType() const override;
    };

  class LimitOrder : public Order {
    public:
      LimitOrder(int trader_id, double price, int quantity) : Order(trader_id, price, quantity, OrderType::SELL) {}
      std::string getOrderType() const override;
    };

  class OrderFactory {
    public:
      virtual std::unique_ptr<Order> createOrder(int, double, int) = 0;
      virtual ~OrderFactory();
  };

  class MarketOrderFactory : public OrderFactory {
    public:
      std::unique_ptr<Order> createOrder(int, double, int) override;
  };

  class LimitOrderFactory : public OrderFactory {
    public:
      std::unique_ptr<Order> createOrder(int, double, int) override;
  };

  class OrderMatchingStrategy {
    public:
      virtual void matchOrders(std::vector<std::shared_ptr<Order>>&, std::vector<std::shared_ptr<Order>>&) = 0;
      virtual ~OrderMatchingStrategy();
  };

  class PriceTimeOrderMatchingStrategy : public OrderMatchingStrategy {
    public:
      void matchOrders(std::vector<std::shared_ptr<Order>>&, std::vector<std::shared_ptr<Order>>&) override;
  };

} // TradingSystemClasses

#endif // SORDER_H