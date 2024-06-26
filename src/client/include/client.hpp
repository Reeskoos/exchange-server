#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

class Client {
 public:
  Client(const std::string& host, short port);
  void run();

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
};

#endif  // CLIENT_HPP
