#include "core.hpp"

Core::Core() {
  trades_[{"USD", "RUB"}] =
      std::make_unique<Trade>(*this, std::make_pair("USD", "RUB"));
  trades_[{"RUB", "USD"}] =
      std::make_unique<Trade>(*this, std::make_pair("RUB", "USD"));
}

std::size_t Core::RegisterNewUser(const std::string& user_name) {
  std::size_t user_id = genUserID(user_name);

  auto peer = std::make_shared<Peer>(user_id, user_name);
  {
    std::lock_guard<std::mutex> lock(peersMutex_);
    peers_[user_id] = peer;
  }
  return user_id;
}

std::string Core::GetUserName(std::size_t user_id) {
  std::lock_guard<std::mutex> lock(peersMutex_);
  auto it = peers_.find(user_id);
  if (it != peers_.end()) {
    return it->second->get_username();
  }
  throw std::runtime_error("Username not found");
}

void Core::AddPeer(std::size_t user_id, std::shared_ptr<Peer> peer) {
  std::lock_guard<std::mutex> lock(peersMutex_);
  peers_[user_id] = peer;
}

void Core::RemovePeer(std::size_t user_id) {
  std::lock_guard<std::mutex> lock(peersMutex_);
  peers_.erase(user_id);
}

std::size_t Core::genUserID(const std::string& user_name) {
  std::hash<std::string> hash_fn;
  std::size_t user_id =
      hash_fn(user_name +
              std::to_string(
                  std::chrono::system_clock::now().time_since_epoch().count()));
  return user_id;
}

std::shared_ptr<Peer> Core::GetPeer(std::size_t user_id) {
  std::lock_guard<std::mutex> lock(peersMutex_);
  auto it = peers_.find(user_id);
  if (it != peers_.end()) {
    return it->second;
  }
  throw std::runtime_error("User ID: " + std::to_string(user_id) +
                           " not found");
}

Trade& Core::getTradeModule(const CurrencyPair& pair) {
  std::lock_guard<std::mutex> lock(tradesMutex_);
  auto it = trades_.find(pair);
  if (it != trades_.end()) {
    return *(it->second);
  }
  throw std::runtime_error("Trade pair: " + pair.first + "/" + pair.second +
                           " not found.");
}

const Core::TradesUnorderedMap& Core::getTradeModules() const {
  std::lock_guard<std::mutex> lock(tradesMutex_);
  return trades_;
}
