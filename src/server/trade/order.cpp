#include "order.hpp"

Order::Order(std::size_t client_id, std::size_t volume,
             const std::string& price, Side side, const CurrencyPair& pair)
    : client_id_(client_id),
      volume_(volume),
      price_(price),
      side_(side),
      currencyPair_(pair),
      timestamp_(std::chrono::steady_clock::now()) {}

bool Order::operator>(const Order& other) const {
  if (side_ == Side::BUY) {
    return (price_ < other.price_) ||
           (price_ == other.price_ && timestamp_ > other.timestamp_);
  } else {
    return (price_ > other.price_) ||
           (price_ == other.price_ && timestamp_ > other.timestamp_);
  }
}

bool Order::operator<(const Order& other) const {
  if (side_ == Side::BUY) {
    return (price_ > other.price_) ||
           (price_ == other.price_ && timestamp_ < other.timestamp_);
  } else {
    return (price_ < other.price_) ||
           (price_ == other.price_ && timestamp_ < other.timestamp_);
  }
}

void Order::reduceVolume(std::size_t value) { volume_ -= value; }

std::size_t Order::getClientId() const { return client_id_; }

std::size_t Order::getVolume() const { return volume_; }

price_t Order::getPrice() const { return price_; }

Side Order::getSide() const { return side_; }

time_point Order::getTimeStamp() const { return timestamp_; }

CurrencyPair Order::getCurrencyPair() const { return currencyPair_; }