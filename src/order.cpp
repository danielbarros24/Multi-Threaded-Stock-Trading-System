#include "../include/order.h"
#include <algorithm>
#include <stdexcept>
#include <utility> 

using namespace TradingSystemClasses;

Order::Order()
{
}

Order::Order(int id, double p, int q, OrderType type)
    : trader_id{id}
    , price{p}
    , quantity{q}
    , timestamp{std::time(nullptr)}
    , ordertype{type}
{
  if (q <= 0) {
    throw std::invalid_argument{"Order quantity must be positive"};
  } else if (p < 0.0) {
    throw std::invalid_argument{"Price cannot be negative"};
  }
}

Order::~Order()=default;

int Order::getTraderId() const {
  return trader_id;
}

double Order::getPrice() const {
  return price;
}

int Order::getQuantity() const {
  return quantity;
}

std::time_t Order::getTimestamp() const {
  return timestamp;
}

void Order::reduceQuantity(int amount) {
  if (amount <= 0) {
    throw std::invalid_argument{"Amount to reduce must be positive!"};
  } else if (amount > quantity) {
    throw std::out_of_range{"Reduction amount exceeds current quantity!"};
  }

  quantity = quantity - amount;
}

OrderType Order::getOrdertype() const {
  return ordertype;
}

void Order::setOrderType(OrderType type) {
    ordertype = type;
}

// MarketOrder
std::string MarketOrder::getOrderType() const {
    return "Market Order";
}

// LimitOrder
std::string LimitOrder::getOrderType() const {
    return "Limit Order";
}

// OrderFactory
OrderFactory::~OrderFactory() = default;

std::unique_ptr<Order> MarketOrderFactory::createOrder(int trader_id, double price, int quantity) {
    return std::make_unique<MarketOrder>(trader_id, price, quantity);
}

std::unique_ptr<Order> LimitOrderFactory::createOrder(int trader_id, double price, int quantity) {
    return std::make_unique<LimitOrder>(trader_id, price, quantity);
}

// OrderMatchingStrategy
OrderMatchingStrategy::~OrderMatchingStrategy() = default;

void PriceTimeOrderMatchingStrategy::matchOrders(std::vector<std::shared_ptr<Order>>& buyOrders, std::vector<std::shared_ptr<Order>>& sellOrders)
{
    // Sort Buy Orders by Ascendant Price and Timestamp
    std::sort(buyOrders.begin(), buyOrders.end(), [](const auto& a, const auto& b) {
        return (a->getPrice() > b->getPrice()) || (a->getPrice() == b->getPrice() && a->getTimestamp() < b->getTimestamp());
    });

    // Sort Sell Orders by Ascendant Price and Timestamp
    std::sort(sellOrders.begin(), sellOrders.end(), [](const auto& a, const auto& b) {
        return (a->getPrice() < b->getPrice()) || (a->getPrice() == b->getPrice() && a->getTimestamp() < b->getTimestamp());
    });

    // Combine Buy and Sell Orders
    for (auto sellOrder = sellOrders.begin(); sellOrder != sellOrders.end();) {
        bool matched = false;
        for (auto buyOrder = buyOrders.begin(); buyOrder != buyOrders.end();) {
            if ((*sellOrder)->getPrice() <= (*buyOrder)->getPrice()) {
                matched = true;

                // Determine Qunatity to be Combined
                int matchQuantity = std::min((*sellOrder)->getQuantity(), (*buyOrder)->getQuantity());

                // Reduce Quantities of Combined Orders
                (*sellOrder)->reduceQuantity(matchQuantity);
                (*buyOrder)->reduceQuantity(matchQuantity);

                // If Buy Order is Completely Consumed, Remove it
                if ((*buyOrder)->getQuantity() == 0) {
                    buyOrder = buyOrders.erase(buyOrder);
                } else {
                    ++buyOrder;
                }

                // If Sell Order is Completely Consumed, Remove and Finish it
                if ((*sellOrder)->getQuantity() == 0) {
                    sellOrder = sellOrders.erase(sellOrder);
                    break;
                }
            } else {
                ++buyOrder;
            }
        }

        // Move on to the Next Sell Order
        if (!matched) {
            ++sellOrder;
        }
    }
}