#ifndef CLIENTSERVERECN_SERVER_TRADE_HPP
#define CLIENTSERVERECN_SERVER_TRADE_HPP

#include <boost/asio.hpp>
#include <chrono>
#include <cmath>
#include <map>
#include <mutex>
#include <queue>
#include <string>

#include "core.hpp"
#include "order.hpp"
#include "types.hpp"

using namespace Types;

class Core;

class Trade {
 public:
  using BuyOrderQueue =
      std::priority_queue<Order, std::vector<Order>, std::less<>>;
  using SellOrderQueue =
      std::priority_queue<Order, std::vector<Order>, std::greater<>>;

  Trade(Core& core, const CurrencyPair& pair);
  void AddOrder(std::size_t client_id, std::size_t volume,
                const std::string& price, Side side, const CurrencyPair& pair);
  void MatchOrders();

  CurrencyPair getCurrencyPair() const;

 private:
  Core& core_;
  CurrencyPair currencyPair_;

  std::mutex ordersMutex_;
  BuyOrderQueue buyOrders_;
  SellOrderQueue sellOrders_;

  void updateBalances(const Order& buyOrder, const Order& sellOrder,
                      price_t tradePrice, std::size_t tradeVolume);
};

#endif  // CLIENTSERVERECN_SERVER_TRADE_HPP