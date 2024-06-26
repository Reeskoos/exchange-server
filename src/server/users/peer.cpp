#include "peer.hpp"

Peer::Peer(std::size_t id, const std::string& username)
    : id_(id), username_(username) {
  balances_["USD"] = 0;
  balances_["RUB"] = 0;
}

std::size_t Peer::get_id() const { return id_; }

std::string Peer::get_username() const { return username_; }

void Peer::deposit(const std::string& currency, price_t amount) {
  balances_[currency] += amount;
}

void Peer::withdraw(const std::string& currency, price_t amount) {
  balances_[currency] -= amount;
}

price_t Peer::get_balance(const std::string& currency) const {
  auto it = balances_.find(currency);
  if (it != balances_.end()) {
    return it->second;
  }
  return 0;
}

json Peer::get_all_balances() const {
  nlohmann::json json_balances;
  for (const auto& balance : balances_) {
    json_balances[balance.first] = balance.second.str();
  }
  return json_balances;
}

void Peer::updateBalances(const std::string& buyCurrency, price_t buyAmount,
                          const std::string& sellCurrency, price_t sellAmount) {
  deposit(buyCurrency, buyAmount);
  withdraw(sellCurrency, sellAmount);
}
