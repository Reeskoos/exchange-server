#include <gtest/gtest.h>

#include "core.hpp"
#include "session.hpp"

class SessionTest : public ::testing::Test {
 protected:
  boost::asio::io_service io_service;
  Core core;
  std::shared_ptr<Session> session;
  tcp::acceptor acceptor;
  tcp::socket client_socket;

  SessionTest()
      : acceptor(io_service, tcp::endpoint(tcp::v4(), 0)),
        client_socket(io_service) {}

  void SetUp() override {
    session = std::make_shared<Session>(io_service, core);
    acceptor.listen();
  }

  void TearDown() override { io_service.stop(); }

  void connect_client() {
    tcp::endpoint endpoint = acceptor.local_endpoint();
    client_socket.connect(endpoint);
  }
};

TEST_F(SessionTest, Socket) {
  ASSERT_NO_THROW({
    tcp::socket& sock = session->socket();
    ASSERT_FALSE(sock.is_open());
  });
}

TEST_F(SessionTest, Start) { ASSERT_NO_THROW(session->start()); }

TEST_F(SessionTest, GetClientID) {
  std::size_t client_id = session->getClientID();
  ASSERT_GE(client_id, 0);
}
