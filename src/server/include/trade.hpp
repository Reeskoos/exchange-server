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

struct TradeData {
  std::size_t volume;
  std::string price;
  CurrencyPair currency_pair;
  Side side;

  TradeData(std::size_t volume, std::string price, CurrencyPair pair, Side side)
      : volume(volume),
        price(std::move(price)),
        currency_pair(std::move(pair)),
        side(side) {}
};

class Trade {
 public:
  using BuyOrderQueue =
      std::priority_queue<Order, std::vector<Order>, std::less<Order>>;
  using SellOrderQueue =
      std::priority_queue<Order, std::vector<Order>, std::greater<Order>>;

  Trade(Core& core, const CurrencyPair& pair);
  ~Trade() = default;

  void AddOrder(std::size_t client_id, std::size_t volume,
                const std::string& price, Side side, const CurrencyPair& pair);
  void MatchOrders();

  CurrencyPair getCurrencyPair() const;
  std::vector<OrderData> getAllActiveOrders();
  std::vector<TradeData> getAllCompletedTrades();

 private:
  Core& core_;
  CurrencyPair currencyPair_;

  std::mutex ordersMutex_;
  std::mutex completedTradesMutex_;

  BuyOrderQueue buyOrders_;
  SellOrderQueue sellOrders_;
  std::vector<TradeData> completedTrades_;

  void updateBalances(const Order& buyOrder, const Order& sellOrder,
                      price_t tradePrice, std::size_t tradeVolume);

  void logMatchedTrade(std::size_t buyerID, std::size_t sellerID,
                       std::size_t volume, price_t& price);
};

#endif  // CLIENTSERVERECN_SERVER_TRADE_HPP