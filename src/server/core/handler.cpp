#include "common.hpp"
#include "handler.hpp"
#include "spdlog/spdlog.h"

Handler::Handler(Core& core, tcp::socket& socket)
    : core_(core), socket_(socket), client_id_(0) {}

void Handler::handleRequest(const std::string& request) {
  auto json_req = json::parse(request);

  if (json_req["ReqType"] == Requests::Registration) {
    handleRegistration(json_req);
  } else if (json_req["ReqType"] == Requests::Buy) {
    handleBuySell(json_req, true);
  } else if (json_req["ReqType"] == Requests::Sell) {
    handleBuySell(json_req, false);
  } else if (json_req["ReqType"] == Requests::Balance) {
    handleBalance();
  }
}

void Handler::handleRegistration(const nlohmann::json& json_req) {
  try {
    std::string user_name = json_req["Message"];
    client_id_ = core_.RegisterNewUser(user_name);
    peer_ = core_.GetPeer(client_id_);
    core_.AddPeer(client_id_, peer_);
    std::string response = std::to_string(client_id_);
    async_write_response(response);
    spdlog::info("User {} registered. Client ID: {}", user_name, client_id_);
  } catch (const std::exception& e) {
    spdlog::error("Exception in handleRegistration: {}", e.what());
    async_write_response("[ERROR] Internal server error.");
  }
}

void Handler::handleBuySell(const nlohmann::json& json_req, bool isBuy) {
  try {
    auto message = json::parse(json_req["Message"].get<std::string>());
    std::size_t volume = std::stoul(message["Volume"].get<std::string>());
    std::string price = message["Price"].get<std::string>();
    std::string base_currency = message["BaseCurrency"].get<std::string>();
    std::string quote_currency = message["QuoteCurrency"].get<std::string>();

    Side side = isBuy ? Side::BUY : Side::SELL;
    CurrencyPair currency_pair = std::make_pair(base_currency, quote_currency);

    core_.getTradeModule(currency_pair)
        .AddOrder(client_id_, volume, price, side, currency_pair);
    std::string response = "Order added.";
    async_write_response(response);
  } catch (const std::exception& e) {
    spdlog::error("Exception in handleBuySell: {}", e.what());
    async_write_response("[ERROR] Internal server error.");
  }
}

void Handler::handleBalance() {
  try {
    auto balances = peer_->get_all_balances();
    std::string response = balances.dump();
    async_write_response(response);
  } catch (const std::exception& e) {
    spdlog::error("Exception in handleBalance: {}", e.what());
    async_write_response("[ERROR] Internal server error.");
  }
}

void Handler::async_write_response(const std::string& response) {
  auto self(shared_from_this());
  boost::asio::async_write(
      socket_, boost::asio::buffer(response + "\0", response.size() + 1),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (ec) {
          spdlog::error("Error writing response: {}", ec.message());
        }
      });
}