#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>

namespace TradingSystemClasses {
  class Stock {
    public:
          Stock();
          void setName(const std::string&);
          void setPrice(double);
          std::string getName();
          double getPrice();
    private:
          std::string name;
          double price;
  };
}


#endif //STOCK_H