#ifndef CLIENTSERVERECN_CLIENT_HPP
#define CLIENTSERVERECN_CLIENT_HPP

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <gtest/gtest.h> 

class Client {
 public:
  Client(const std::string& host, short port);
  void run();
  void stop();

 private:
  void connect();
  void sendMessage(const std::string& id, const std::string& requestType,
                   const std::string& message);
  std::string readMessage();
  void processRegistration();
  void menu();
  void placeOrder(bool isBuy);
  void displayBalance(const std::string& balance_response);

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  std::string host_;
  short port_;
  std::string client_id_;

  FRIEND_TEST(ClientTest, RegisterClient);
  FRIEND_TEST(ClientTest, PlaceBuyOrder);
  FRIEND_TEST(ClientTest, PlaceSellOrder);
  FRIEND_TEST(ClientTest, CheckClientBalance);
  FRIEND_TEST(ClientTest, ConnectClient);
};

#endif  // CLIENTSERVERECN_CLIENT_HPP
