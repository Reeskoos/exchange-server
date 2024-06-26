#ifndef CLIENTSERVERECN_SERVER_HPP
#define CLIENTSERVERECN_SERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "common.hpp"
#include "core.hpp"
#include "session.hpp"

using boost::asio::ip::tcp;

class Server {
 public:
  explicit Server(boost::asio::io_service& io_service);
  ~Server();
  void run();

 private:
  void do_accept();
  void handle_accept(std::shared_ptr<Session> new_session,
                     const boost::system::error_code& error);
  void start_match_orders();
  void match_orders_periodically(const boost::system::error_code& error);

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  Core core_;
  std::vector<std::thread> worker_threads_;
  boost::asio::steady_timer timer_;
  bool running_;
  std::mutex match_mutex_;
};

#endif  // CLIENTSERVERECN_SERVER_HPP
