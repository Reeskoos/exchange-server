#ifndef CLIENTSERVERECN_SERVER_SESSION_HPP
#define CLIENTSERVERECN_SERVER_SESSION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "core.hpp"
#include "handler.hpp"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(boost::asio::io_service& io_service, Core& core);
  tcp::socket& socket();
  void start();

  std::size_t getClientID() const;

 private:
  void do_read();
  void handle_read(const boost::system::error_code& error,
                   std::size_t bytes_transferred);
  void async_write(const std::string& response);

  tcp::socket socket_;
  Core& core_;
  std::size_t client_id_;
  std::shared_ptr<Handler> handler_;  
  enum { max_length = 1024 };
  char data_[max_length];
};

#endif  // CLIENTSERVERECN_SERVER_SESSION_HPP
