#ifndef CLIENTSERVERECN_SERVER_ORDER_HPP
#define CLIENTSERVERECN_SERVER_ORDER_HPP

#include "types.hpp"

using namespace Types;

enum class Side { BUY, SELL };

class Order {
 public:
  Order(std::size_t client_id, std::size_t volume, const std::string& price,
        Side side, const CurrencyPair& pair);

  void reduceVolume(std::size_t value);

  std::size_t getClientId() const;
  std::size_t getVolume() const;
  price_t getPrice() const;
  Side getSide() const;
  CurrencyPair getCurrencyPair() const;
  time_point getTimeStamp() const;

  bool operator>(const Order& other) const;
  bool operator<(const Order& other) const;

 private:
  std::size_t client_id_;
  std::size_t volume_;
  price_t price_;
  Side side_;
  CurrencyPair currencyPair_;
  time_point timestamp_;
};

#endif  // CLIENTSERVERECN_SERVER_ORDER_HPP