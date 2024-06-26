#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <thread>
#include "client.hpp"
#include "server.hpp"

class ClientTest : public ::testing::Test {
protected:
    boost::asio::io_service io_service;
    std::unique_ptr<Server> server;
    std::unique_ptr<std::thread> server_thread;
    short port;

    ClientTest() : port(0) {}

    void SetUp() override {
        port = 25565;

        server = std::make_unique<Server>(io_service, port);
        server_thread = std::make_unique<std::thread>([this]() {
            io_service.run();
        });
    }

    void TearDown() override {
        io_service.stop();
        if (server_thread->joinable()) {
            server_thread->join();
        }
    }
};

TEST_F(ClientTest, RegisterClient) {
    Client client("127.0.0.1", port);
    client.connect();
    std::string username = "test_user";
    client.sendMessage("0", "Reg", username);
    std::string response = client.readMessage();
    ASSERT_FALSE(response.empty());
    client.stop(); // Добавлено
}

TEST_F(ClientTest, PlaceBuyOrder) {
    Client client("127.0.0.1", port);
    client.connect();
    std::string username = "test_user";
    client.sendMessage("0", "Reg", username);
    std::string response = client.readMessage();
    ASSERT_FALSE(response.empty());

    std::string client_id = response;

    // Place a buy order
    json buy_order = {
        {"Volume", "100"},
        {"Price", "50.5"},
        {"BaseCurrency", "USD"},
        {"QuoteCurrency", "RUB"}
    };
    client.sendMessage(client_id, "Buy", buy_order.dump());
    std::string buy_response = client.readMessage();
    ASSERT_FALSE(buy_response.empty());
    client.stop(); // Добавлено
}

TEST_F(ClientTest, PlaceSellOrder) {
    Client client("127.0.0.1", port);
    client.connect();
    std::string username = "test_user";
    client.sendMessage("0", "Reg", username);
    std::string response = client.readMessage();
    ASSERT_FALSE(response.empty());

    std::string client_id = response;

    // Place a sell order
    json sell_order = {
        {"Volume", "100"},
        {"Price", "50.5"},
        {"BaseCurrency", "USD"},
        {"QuoteCurrency", "RUB"}
    };
    client.sendMessage(client_id, Requests::Sell, sell_order.dump());
    std::string sell_response = client.readMessage();
    ASSERT_FALSE(sell_response.empty());
    client.stop(); // Добавлено
}

TEST_F(ClientTest, CheckClientBalance) {
    Client client("127.0.0.1", port);
    client.connect();
    std::string username = "test_user";
    client.sendMessage("0", "Reg", username);
    std::string response = client.readMessage();
    ASSERT_FALSE(response.empty());

    std::string client_id = response;

  
    client.sendMessage(client_id, "Bal", "");
    std::string balance_response = client.readMessage();
    ASSERT_FALSE(balance_response.empty());
    client.stop(); 
}

TEST_F(ClientTest, ConnectClient) {
    ASSERT_NO_THROW({
        Client client("127.0.0.1", port);
        client.connect();
        client.stop();
    });
}
