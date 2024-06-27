#ifndef CLIENTSERVERECN_SERVER_ORDER_HPP
#define CLIENTSERVERECN_SERVER_ORDER_HPP

#include "types.hpp"

using namespace Types;

enum class Side { BUY, SELL };

struct OrderData {
 public:
  std::size_t client_id_;
  std::size_t volume_;
  price_t price_;
  Side side_;
  CurrencyPair currency_pair_;
  time_point timestamp_;

  OrderData(std::size_t client_id, std::size_t volume, std::string price,
            Side side, CurrencyPair currency_pair)
      : client_id_(client_id),
        volume_(volume),
        price_(std::move(price)),
        side_(side),
        currency_pair_(std::move(currency_pair)),
        timestamp_(std::chrono::steady_clock::now()) {}

  bool operator<(const OrderData& other) const;
  bool operator>(const OrderData& other) const;
};

class Order {
 public:
  Order(std::size_t client_id, std::size_t volume, const std::string& price,
        Side side, const CurrencyPair& pair);

  void reduceVolume(std::size_t value);

  OrderData getData() const;
  std::size_t getClientId() const;
  std::size_t getVolume() const;
  price_t getPrice() const;
  Side getSide() const;
  CurrencyPair getCurrencyPair() const;
  time_point getTimeStamp() const;

  bool operator>(const Order& other) const;
  bool operator<(const Order& other) const;

 private:
  OrderData data_;
};

#endif  // CLIENTSERVERECN_SERVER_ORDER_HPP
