#ifndef CLIENTSERVERECN_SERVER_CORE_HPP
#define CLIENTSERVERECN_SERVER_CORE_HPP

#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "peer.hpp"
#include "trade.hpp"
#include "types.hpp"

using namespace Types;

class Trade;

class Core {
 public:
  using TradePtr = std::unique_ptr<Trade>;
  using Hash = boost::hash<CurrencyPair>;
  using TradesUnorderedMap = std::unordered_map<CurrencyPair, TradePtr, Hash>;
  
  using PeerUnorderedMap =
      std::unordered_map<std::size_t, std::shared_ptr<Peer>>;

  Core();
  ~Core() = default;

  std::size_t RegisterNewUser(const std::string& user_name);
  std::string GetUserName(std::size_t user_id);
  void AddPeer(std::size_t user_id, std::shared_ptr<Peer> peer);
  void RemovePeer(std::size_t user_id);
  std::shared_ptr<Peer> GetPeer(std::size_t user_id);

  Trade& getTradeModule(const CurrencyPair& pair);
  const TradesUnorderedMap& getTradeModules() const;

 private:
  TradesUnorderedMap trades_;
  PeerUnorderedMap peers_;

  mutable std::mutex peersMutex_;
  mutable std::mutex tradesMutex_;

  std::size_t genUserID(const std::string& user_name);
};

#endif  // CLIENTSERVERECN_SERVER_CORE_HPP
