#include <boost/bind/bind.hpp>
#include <nlohmann/json.hpp>
#include "session.hpp"
#include "spdlog/spdlog.h"

Session::Session(boost::asio::io_service& io_service, Core& core)
    : socket_(io_service),
      core_(core),
      client_id_(0),
      handler_(std::make_shared<Handler>(core, socket_)) {}

tcp::socket& Session::socket() { return socket_; }

void Session::start() {
  do_read();
}

void Session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      boost::asio::buffer(data_, max_length),
      [this, self](const boost::system::error_code& ec, std::size_t length) {
        handle_read(ec, length);
      });
}

void Session::handle_read(const boost::system::error_code& error,
                          std::size_t bytes_transferred) {
  if (!error) {
    std::string request(data_, bytes_transferred);
    spdlog::info("Received request: {}", request);
    try {
      handler_->handleRequest(request);
    } catch (const std::exception& e) {
      std::string response = "[ERROR]: Internal server error.";
      spdlog::error("Exception in request handler: {}", e.what());
      async_write(response);
    }
    do_read();
  } else {
    core_.RemovePeer(client_id_);
  }
}

void Session::async_write(const std::string& response) {
  auto self(shared_from_this());
  boost::asio::async_write(
      socket_, boost::asio::buffer(response + "\0", response.size() + 1),
      [this, self](boost::system::error_code ec,
                   std::size_t /*length*/) {
        if (ec) {
          spdlog::error("Error writing to socket: {}", ec.message());
          core_.RemovePeer(client_id_);
        }
      });
}

std::size_t Session::getClientID() const { return client_id_; }
