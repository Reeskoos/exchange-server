
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <string>

#include "client.hpp"
#include "common.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

Client::Client(const std::string& host, short port)
    : socket_(io_service_), host_(host), port_(port) {}

void Client::connect() {
  try {
    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(tcp::v4(), host_, std::to_string(port_));
    tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::connect(socket_, iterator);
  } catch (const boost::system::system_error&) {
    std::cerr << "Failed to connect to the server. Please check your network "
                 "and server status."
              << std::endl;
    exit(1);
  }
}

void Client::sendMessage(const std::string& id, const std::string& requestType,
                         const std::string& message) {
  try {
    json req;
    req["UserId"] = id;
    req["ReqType"] = requestType;
    req["Message"] = message;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
  } catch (...) {
    std::cerr << "Failed to send message. Please try again later." << std::endl;
  }
}

std::string Client::readMessage() {
  try {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket_, buffer, "\0");
    std::istream is(&buffer);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
  } catch (...) {
    std::cerr << "Failed to read message. Please try again later." << std::endl;
    return "";
  }
}

void Client::processRegistration() {
  std::string name;
  std::cout << "Hello! Enter your name: ";
  std::cin >> name;

  sendMessage("0", Requests::Registration, name);
  client_id_ = readMessage();
}

void Client::placeOrder(bool isBuy) {
  std::string volume, price, base_currency, quote_currency;

  std::regex number_pattern(
      "^[1-9]\\d*$");  // Pattern for positive integers without leading zeros

  std::cout << "Enter volume: ";
  while (true) {
    std::cin >> volume;
    if (std::regex_match(volume, number_pattern)) {
      break;
    } else {
      std::cout << "Invalid input. Enter a positive number without leading "
                   "zeros for volume: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::regex decimal_pattern("^([1-9]\\d*|0)(\\.\\d+)?$");

  std::cout << "Enter price: ";
  while (true) {
    std::cin >> price;
    if (std::regex_match(price, decimal_pattern)) {
      break;
    } else {
      std::cout << "Invalid input. Enter a positive number for price: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  int currency_option = 0;
  std::cout << "Choose currency pair:\n"
               "1) USD/RUB\n"
               "2) RUB/USD\n";

  while (true) {
    std::cin >> currency_option;
    if (std::cin.fail() || (currency_option != 1 && currency_option != 2)) {
      std::cout << "Invalid option. Choose 1 or 2: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      break;
    }
  }

  if (currency_option == 1) {
    base_currency = "USD";
    quote_currency = "RUB";
  } else {
    base_currency = "RUB";
    quote_currency = "USD";
  }

  json message{{"Volume", volume},
               {"Price", price},
               {"BaseCurrency", base_currency},
               {"QuoteCurrency", quote_currency}};

  std::string requestType = isBuy ? Requests::Buy : Requests::Sell;
  sendMessage(client_id_, requestType, message.dump());
  std::cout << readMessage() << std::endl;
}

void Client::displayBalance(const std::string& balance_response) {
  try {
    auto balances = json::parse(balance_response);
    std::cout << "Current Balances:" << std::endl;
    for (const auto& [currency, balance] : balances.items()) {
      std::cout << " - " << currency << ": " << balance << std::endl;
    }
    std::cout << std::endl;
  } catch (...) {
    std::cerr << "Failed to parse balance response." << std::endl;
  }
}

void Client::viewActiveOrders() {
  try {
    auto [base_currency, quote_currency] = inputCurrencyPair();
    json message{{"BaseCurrency", base_currency},
                 {"QuoteCurrency", quote_currency}};
    sendMessage(client_id_, Requests::ActiveOrders, message.dump());
    std::string response = readMessage();
    displayOrders(response, "Active Orders");
  } catch (...) {
    std::cerr << "Failed to retrieve active orders. Please try again later."
              << std::endl;
  }
}

void Client::viewCompletedTrades() {
  try {
    auto [base_currency, quote_currency] = inputCurrencyPair();
    json message{{"BaseCurrency", base_currency},
                 {"QuoteCurrency", quote_currency}};
    sendMessage(client_id_, Requests::CompletedTrades, message.dump());
    std::string response = readMessage();
    displayOrders(response, "Completed Trades");
  } catch (...) {
    std::cerr << "Failed to retrieve completed trades. Please try again later."
              << std::endl;
  }
}

void Client::displayOrders(const std::string& orders_response,
                           const std::string& title) {
  try {
    auto orders = json::parse(orders_response);
    if (orders.empty()) {
      if (title == "Active Orders") {
        std::cout << "There's no active orders." << std::endl;
      } else if (title == "Completed Trades") {
        std::cout << "There's no completed trades." << std::endl;
      }
      return;
    }

    std::cout << title << ":" << std::endl;
    for (const auto& order : orders) {
      std::string currency_pair;

      if (order["currency_pair"].is_array()) {
        currency_pair = order["currency_pair"][0].get<std::string>() + "/" +
                        order["currency_pair"][1].get<std::string>();
      } else {
        currency_pair = order["currency_pair"];
      }

      std::cout << " - Volume: " << order["volume"]
                << ", Price: " << order["price"] << ", " << order["side"]
                << ", " << currency_pair << "." << std::endl;
    }
    std::cout << std::endl;
  } catch (...) {
    std::cerr << "Failed to parse orders response." << std::endl;
  }
}

std::pair<std::string, std::string> Client::inputCurrencyPair() {
  int currency_option = 0;
  std::cout << "Choose currency pair:\n"
               "1) USD/RUB\n"
               "2) RUB/USD\n";

  while (true) {
    std::cin >> currency_option;
    if (std::cin.fail() || (currency_option != 1 && currency_option != 2)) {
      std::cout << "Invalid option. Choose 1 or 2: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      break;
    }
  }

  if (currency_option == 1) {
    return {"USD", "RUB"};
  } else {
    return {"RUB", "USD"};
  }
}

void Client::menu() {
  while (true) {
    std::cout << "Menu:\n"
                 "1) Balance\n"
                 "2) Place Buy Order\n"
                 "3) Place Sell Order\n"
                 "4) View Active Orders\n"
                 "5) View Completed Trades\n"
                 "6) Exit\n"
              << std::endl;

    int menu_option_num = 0;
    std::cout << "Choose an option: ";
    while (true) {
      std::cin >> menu_option_num;
      if (std::cin.fail() || (menu_option_num < 1 || menu_option_num > 6)) {
        std::cout << "Invalid option. Choose 1, 2, 3, 4, 5, or 6: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      } else {
        break;
      }
    }

    switch (menu_option_num) {
      case 1: {
        sendMessage(client_id_, Requests::Balance, "");
        std::string balance_response = readMessage();
        displayBalance(balance_response);
        break;
      }
      case 2: {  // buy
        placeOrder(true);
        break;
      }
      case 3: {  // sell
        placeOrder(false);
        break;
      }
      case 4: {
        viewActiveOrders();
        break;
      }
      case 5: {
        viewCompletedTrades();
        break;
      }
      case 6: {
        std::cout << "Exiting..." << std::endl;
        socket_.close();
        exit(0);
      }
      default: {
        std::cout << "Unknown menu option\n" << std::endl;
        break;
      }
    }
  }
}

void Client::run() {
  connect();
  processRegistration();
  menu();
}

void Client::stop() { io_service_.stop(); }