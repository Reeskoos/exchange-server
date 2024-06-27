#include <iostream>

#include "spdlog/spdlog.h"
#include "trade.hpp"

Trade::Trade(Core& core, const CurrencyPair& pair)
    : core_(core), currencyPair_(pair){};

void Trade::AddOrder(std::size_t client_id, std::size_t volume,
                     const std::string& price, Side side,
                     const CurrencyPair& pair) {
  if (!volume || price_t(price) <= 0) {
    throw std::runtime_error("Invalid order");
  }

  Order order(client_id, volume, price, side, pair);

  std::lock_guard<std::mutex> lock(ordersMutex_);
  if (order.getSide() == Side::BUY) {
    buyOrders_.push(order);
  } else {
    sellOrders_.push(order);
  }
}

void Trade::MatchOrders() {
  std::lock_guard<std::mutex> lock(ordersMutex_);

  while (!buyOrders_.empty() && !sellOrders_.empty()) {
    try {
      Order buyOrder = buyOrders_.top();
      Order sellOrder = sellOrders_.top();

      if (buyOrder.getClientId() == sellOrder.getClientId()) {
        break;
      }

      if (buyOrder.getPrice() >= sellOrder.getPrice()) {
        price_t tradePrice = sellOrder.getTimeStamp() < buyOrder.getTimeStamp()
                                 ? sellOrder.getPrice()
                                 : buyOrder.getPrice();

        std::size_t tradeVolume =
            std::min(buyOrder.getVolume(), sellOrder.getVolume());

        updateBalances(buyOrder, sellOrder, tradePrice, tradeVolume);

        buyOrder.reduceVolume(tradeVolume);
        sellOrder.reduceVolume(tradeVolume);

        buyOrders_.pop();
        sellOrders_.pop();

        if (buyOrder.getVolume() != 0) buyOrders_.push(buyOrder);

        if (sellOrder.getVolume() != 0) sellOrders_.push(sellOrder);

        {
          std::lock_guard<std::mutex> tradesLock(completedTradesMutex_);
          completedTrades_.emplace_back(tradeVolume, tradePrice.str(),
                                        currencyPair_, buyOrder.getSide());
        }

        logMatchedTrade(buyOrder.getClientId(), sellOrder.getClientId(),
                        tradeVolume, tradePrice);

      } else {
        break;
      }
    } catch (const std::exception& e) {
      spdlog::error("Exception in MatchOrders: {}", e.what());
    }
  }
}

void Trade::updateBalances(const Order& buyOrder, const Order& sellOrder,
                           price_t tradePrice, std::size_t tradeVolume) {
  try {
    auto buyer = core_.GetPeer(buyOrder.getClientId());
    auto seller = core_.GetPeer(sellOrder.getClientId());

    if (buyer && seller) {
      price_t totalCost = tradePrice * tradeVolume;
      std::string baseCurrency = currencyPair_.first;
      std::string quoteCurrency = currencyPair_.second;

      buyer->updateBalances(baseCurrency, tradeVolume, quoteCurrency,
                            totalCost);
      seller->updateBalances(quoteCurrency, totalCost, baseCurrency,
                             tradeVolume);
    }
  } catch (const std::exception& e) {
    spdlog::error("Exception in updateBalances: {}", e.what());
  }
}

void Trade::logMatchedTrade(std::size_t buyerID, std::size_t sellerID,
                            std::size_t volume, price_t& price) {
  std::string trade_log = fmt::format(
      "buyer_id: {}, seller_id: {}, volume: {}, price: {}, pair: {}/{}",
      buyerID, sellerID, volume, price.str(), currencyPair_.first,
      currencyPair_.second);

  spdlog::info("Trade matched: {}", trade_log);
}

CurrencyPair Trade::getCurrencyPair() const { return currencyPair_; }

std::vector<OrderData> Trade::getAllActiveOrders() {
  std::vector<OrderData> allActiveOrders;
  std::lock_guard<std::mutex> lock(ordersMutex_);

  auto copyBuyOrders = buyOrders_;
  while (!copyBuyOrders.empty()) {
    allActiveOrders.push_back(copyBuyOrders.top().getData());
    copyBuyOrders.pop();
  }

  auto copySellOrders = sellOrders_;
  while (!copySellOrders.empty()) {
    allActiveOrders.push_back(copySellOrders.top().getData());
    copySellOrders.pop();
  }

  return allActiveOrders;
}

std::vector<TradeData> Trade::getAllCompletedTrades() {
  std::lock_guard<std::mutex> lock(completedTradesMutex_);
  return completedTrades_;
}
