#include <boost/bind/bind.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

#include "common.hpp"
#include "server.hpp"

using boost::asio::ip::tcp;

Core& GetCore() {
  static Core core;
  return core;
}

std::string Core::RegisterNewUser(const std::string& aUserName) {
  size_t newUserId = mUsers.size();
  mUsers[newUserId] = aUserName;
  return std::to_string(newUserId);
}

std::string Core::GetUserName(const std::string& aUserId) {
  const auto userIt = mUsers.find(std::stoi(aUserId));
  if (userIt == mUsers.cend()) {
    return "Error! Unknown User";
  } else {
    return userIt->second;
  }
}

session::session(boost::asio::io_service& io_service) : socket_(io_service) {}

tcp::socket& session::socket() { return socket_; }

void session::start() {
  socket_.async_read_some(
      boost::asio::buffer(data_, max_length),
      boost::bind(&session::handle_read, this, boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& error,
                          size_t bytes_transferred) {
  if (!error) {
    data_[bytes_transferred] = '\0';

    auto j = nlohmann::json::parse(data_);
    auto reqType = j["ReqType"];

    std::string reply = "Error! Unknown request type";
    if (reqType == Requests::Registration) {
      reply = GetCore().RegisterNewUser(j["Message"]);
    } else if (reqType == Requests::Hello) {
      reply = "Hello, " + GetCore().GetUserName(j["UserId"]) + "!\n";
    }

    boost::asio::async_write(socket_, boost::asio::buffer(reply, reply.size()),
                             boost::bind(&session::handle_write, this,
                                         boost::asio::placeholders::error));
  } else {
    delete this;
  }
}

void session::handle_write(const boost::system::error_code& error) {
  if (!error) {
    socket_.async_read_some(
        boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  } else {
    delete this;
  }
}

server::server(boost::asio::io_service& io_service)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "Server started! Listen " << port << " port" << std::endl;

  session* new_session = new session(io_service_);
  acceptor_.async_accept(new_session->socket(),
                         boost::bind(&server::handle_accept, this, new_session,
                                     boost::asio::placeholders::error));
}

void server::handle_accept(session* new_session,
                           const boost::system::error_code& error) {
  if (!error) {
    new_session->start();
    new_session = new session(io_service_);
    acceptor_.async_accept(
        new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
                    boost::asio::placeholders::error));
  } else {
    delete new_session;
  }
}

int main() {
  try {
    boost::asio::io_service io_service;
    static Core core;

    server s(io_service);

    io_service.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
