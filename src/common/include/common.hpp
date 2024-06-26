#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests {
const std::string Registration = "Reg";
const std::string Hello = "Hel";
const std::string Buy = "Buy";
const std::string Sell = "Sel";
const std::string Balance = "Bal";

}  // namespace Requests

#endif  // CLIENSERVERECN_COMMON_HPP
