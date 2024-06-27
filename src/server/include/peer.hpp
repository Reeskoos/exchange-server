#ifndef CLIENTSERVERECN_SERVER_PEER_HPP
#define CLIENTSERVERECN_SERVER_PEER_HPP

#include <boost/asio.hpp>

#include "types.hpp"

using namespace Types;

class Peer {
 public:
  Peer(std::size_t id, const std::string& username);
  ~Peer() = default;

  std::size_t get_id() const;
  std::string get_username() const;

  void deposit(const std::string& currency, price_t amount);
  void withdraw(const std::string& currency, price_t amount);

  price_t get_balance(const std::string& currency) const;
  json get_all_balances() const;
  void updateBalances(const std::string& buyCurrency, price_t buyAmount,
                      const std::string& sellCurrency, price_t sellAmount);

 private:
  std::size_t id_;
  std::string username_;
  std::unordered_map<std::string, price_t> balances_;
};

#endif  // CLIENTSERVERECN_SERVER_PEER_HPP