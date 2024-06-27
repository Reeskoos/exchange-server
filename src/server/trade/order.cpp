#include "order.hpp"

bool OrderData::operator>(const OrderData& other) const {
  return (price_ > other.price_) ||
         (price_ == other.price_ && timestamp_ < other.timestamp_);
}

bool OrderData::operator<(const OrderData& other) const {
  return (price_ < other.price_) ||
         (price_ == other.price_ && timestamp_ < other.timestamp_);
}

Order::Order(std::size_t client_id, std::size_t volume,
             const std::string& price, Side side, const CurrencyPair& pair)
    : data_(client_id, volume, price, side, pair) {}

bool Order::operator>(const Order& other) const { return data_ > other.data_; }

bool Order::operator<(const Order& other) const { return data_ < other.data_; }

void Order::reduceVolume(std::size_t value) {
  if (value > data_.volume_) {
    throw std::logic_error("Reduce volume error");
  }
  data_.volume_ -= value;
}

OrderData Order::getData() const { return data_; }

std::size_t Order::getClientId() const { return data_.client_id_; }

std::size_t Order::getVolume() const { return data_.volume_; }

price_t Order::getPrice() const { return data_.price_; }

Side Order::getSide() const { return data_.side_; }

CurrencyPair Order::getCurrencyPair() const { return data_.currency_pair_; }

time_point Order::getTimeStamp() const { return data_.timestamp_; }
