#ifndef CLIENTSERVERECN_COMMON_HPP
#define CLIENTSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests {
const std::string Registration = "Reg";
const std::string Hello = "Hel";
const std::string Buy = "Buy";
const std::string Sell = "Sel";
const std::string Balance = "Bal";
const std::string ActiveOrders = "Ord";
const std::string CompletedTrades = "Trd";

}  // namespace Requests

#endif  // CLIENTSERVERECN_COMMON_HPP
