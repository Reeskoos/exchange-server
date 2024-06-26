#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "handler.hpp"
#include "core.hpp"
#include "session.hpp"

class HandlerTest : public ::testing::Test {
protected:
    boost::asio::io_service io_service;
    tcp::socket socket;
    Core core;
    std::shared_ptr<Handler> handler;

    HandlerTest() : socket(io_service) {}

    void SetUp() override {
        handler = std::make_shared<Handler>(core, socket);
    }
};

TEST_F(HandlerTest, HandleRegistration) {
    // Mock request for registration
    nlohmann::json request = {
        {"ReqType", "Reg"},
        {"Message", "test_user"}
    };

    std::string request_str = request.dump();
    ASSERT_NO_THROW(handler->handleRequest(request_str));
}

TEST_F(HandlerTest, HandleBuyOrder) {
    // Register user first
    nlohmann::json registration_request = {
        {"ReqType", "Reg"},
        {"Message", "test_user"}
    };
    std::string registration_request_str = registration_request.dump();
    handler->handleRequest(registration_request_str);

    // Mock request for buying order
    nlohmann::json buy_request = {
        {"ReqType", "Buy"},
        {"Message", R"({"Volume": "100", "Price": "50.5", "BaseCurrency": "USD", "QuoteCurrency": "RUB"})"}
    };

    std::string buy_request_str = buy_request.dump();
    ASSERT_NO_THROW(handler->handleRequest(buy_request_str));
}

TEST_F(HandlerTest, HandleSellOrder) {
    // Register user first
    nlohmann::json registration_request = {
        {"ReqType", "Reg"},
        {"Message", "test_user"}
    };
    std::string registration_request_str = registration_request.dump();
    handler->handleRequest(registration_request_str);

    // Mock request for selling order
    nlohmann::json sell_request = {
        {"ReqType", "Sel"},
        {"Message", R"({"Volume": "100", "Price": "50.5", "BaseCurrency": "USD", "QuoteCurrency": "RUB"})"}
    };

    std::string sell_request_str = sell_request.dump();
    ASSERT_NO_THROW(handler->handleRequest(sell_request_str));
}

TEST_F(HandlerTest, HandleBalance) {
    // Register user first
    nlohmann::json registration_request = {
        {"ReqType", "Reg"},
        {"Message", "test_user"}
    };
    std::string registration_request_str = registration_request.dump();
    handler->handleRequest(registration_request_str);

    // Mock request for checking balance
    nlohmann::json balance_request = {
        {"ReqType", "Bal"},
        {"Message", ""}
    };

    std::string balance_request_str = balance_request.dump();
    ASSERT_NO_THROW(handler->handleRequest(balance_request_str));
}
