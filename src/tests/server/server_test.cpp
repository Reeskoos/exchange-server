#include <gtest/gtest.h>

#include "server.hpp"

class ServerTest : public ::testing::Test {
 protected:
  boost::asio::io_service io_service;
  short port = 12345;
  std::unique_ptr<Server> server;
  std::unique_ptr<std::thread> server_thread;
  Core core;

  void SetUp() override {
    server = std::make_unique<Server>(io_service, port);
    server_thread = std::make_unique<std::thread>([this]() { server->run(); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  void TearDown() override {
    io_service.stop();
    if (server_thread && server_thread->joinable()) {
      server_thread->join();
    }
  }
};

TEST_F(ServerTest, ServerStart) { ASSERT_NO_THROW(server->run()); }

TEST_F(ServerTest, ServerAcceptConnection) {
  boost::asio::io_service client_io_service;
  tcp::socket socket(client_io_service);
  tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"),
                         port);

  ASSERT_NO_THROW(socket.connect(endpoint));

  std::this_thread::sleep_for(std::chrono::seconds(1));

  ASSERT_TRUE(socket.is_open());
  socket.close();
}
