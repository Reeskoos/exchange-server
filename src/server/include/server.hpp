#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <map>
#include <string>

using boost::asio::ip::tcp;

class Core {
 public:
  std::string RegisterNewUser(const std::string& aUserName);
  std::string GetUserName(const std::string& aUserId);

 private:
  std::map<std::size_t, std::string> mUsers;
};

class session {
 public:
  session(boost::asio::io_service& io_service);
  tcp::socket& socket();
  void start();

 private:
  void handle_read(const boost::system::error_code& error,
                   std::size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

class server {
 public:
  server(boost::asio::io_service& io_service);

 private:
  void handle_accept(session* new_session,
                     const boost::system::error_code& error);

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

#endif  // SERVER_HPP
