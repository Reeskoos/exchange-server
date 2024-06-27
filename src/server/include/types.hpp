#ifndef CLIENTSERVERECN_SERVER_TYPES_HPP
#define CLIENTSERVERECN_SERVER_TYPES_HPP

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <nlohmann/json.hpp>

namespace Types {
using price_t = boost::multiprecision::cpp_dec_float_50;
using json = nlohmann::json;
using CurrencyPair = std::pair<std::string, std::string>;
using time_point = std::chrono::steady_clock::time_point;
}  // namespace Types

#endif  // CLIENTSERVERECN_SERVER_TYPES_HPP