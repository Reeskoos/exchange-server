#ifndef CLIENTSERVERECN_HANDLER_HPP
#define CLIENTSERVERECN_HANDLER_HPP

#include <boost/asio.hpp>

#include "core.hpp"
#include "types.hpp"

using boost::asio::ip::tcp;

using namespace Types;

class Handler : public std::enable_shared_from_this<Handler> {
 public:
  Handler(Core& core, tcp::socket& socket);

  void handleRequest(const std::string& request);

 private:
  void handleRegistration(const json& json_req);
  void handleBuySell(const json& json_req, bool isBuy);
  void handleBalance();
  void async_write_response(const std::string& response);

  Core& core_;
  std::size_t client_id_;
  tcp::socket& socket_;
  std::shared_ptr<Peer> peer_;
};

#endif  // CLIENTSERVERECN_HANDLER_HPP
