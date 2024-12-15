#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>

namespace TradingSystemClasses {
  class Stock {
    public:
          Stock();
          Stock(double p, const std::string& n);
          void setName(const std::string&);
          void setPrice(double);
          double getPrice() const;
          std::string getName() const;
    private:
          std::string name;
          double price;
  };
}


#endif //STOCK_H