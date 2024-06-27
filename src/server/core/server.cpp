#include <boost/bind/bind.hpp>
#include <iostream>

#include "server.hpp"
#include "session.hpp"
#include "spdlog/spdlog.h"

Server::Server(boost::asio::io_service& io_service)
    : Server(io_service, port) {}

Server::Server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      timer_(io_service),
      running_(true) {
  spdlog::info("Server started! Listening on port {}", port);
  do_accept();
  start_match_orders();
}

Server::~Server() {
  running_ = false;
  io_service_.stop();
  for (auto& thread : worker_threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void Server::do_accept() {
  auto new_session = std::make_shared<Session>(io_service_, core_);
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&Server::handle_accept, this, new_session,
                                     boost::asio::placeholders::error));
}

void Server::handle_accept(std::shared_ptr<Session> new_session,
                           const boost::system::error_code& error) {
  if (!error) {
    spdlog::info("Accepted new session: user_id: {}",
                 new_session->getClientID());
    try {
      new_session->start();
    } catch (const std::exception& e) {
      spdlog::error("Exception in session start: {}", e.what());
    }
  } else {
    spdlog::error("Error accepting session: {}", error.message());
  }
  do_accept();
}

void Server::start_match_orders() {
  timer_.expires_from_now(std::chrono::seconds(1));
  timer_.async_wait(boost::bind(&Server::match_orders_periodically, this,
                                boost::asio::placeholders::error));
}

void Server::match_orders_periodically(const boost::system::error_code& error) {
  if (!error) {
    std::lock_guard<std::mutex> lock(match_mutex_);
    try {
      for (auto& trade_pair : core_.getTradeModules()) {
        trade_pair.second->MatchOrders();
      }
    } catch (const std::exception& e) {
      spdlog::error("Exception in matching orders: {}", e.what());
    }
    start_match_orders();
  } else if (error != boost::asio::error::operation_aborted) {
    spdlog::error("Timer error: {}", error.message());
  }
}

void Server::run() {
  std::size_t thread_count = std::thread::hardware_concurrency();
  if (thread_count == 0) {
    thread_count = 4;
    spdlog::warn("Failed to detect number of CPU cores. Using 4 threads");
  }

  spdlog::info("Starting {} threads for IO service", thread_count);
  for (std::size_t i = 0; i < thread_count; ++i) {
    worker_threads_.emplace_back([this] {
      try {
        io_service_.run();
      } catch (const std::exception& e) {
        spdlog::error("Exception in worker thread: {}", e.what());
      }
    });
  }
}