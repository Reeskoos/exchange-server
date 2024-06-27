#include <gtest/gtest.h>

#include "peer.hpp"

TEST(PeerTest, CreatePeer) {
  Peer peer(1, "test_user");
  ASSERT_EQ(peer.get_id(), 1);
  ASSERT_EQ(peer.get_username(), "test_user");
}

TEST(PeerTest, DepositWithdraw) {
  Peer peer(1, "test_user");
  peer.deposit("USD", price_t("100.5"));
  ASSERT_EQ(peer.get_balance("USD"), price_t("100.5"));

  peer.withdraw("USD", price_t("50.5"));
  ASSERT_EQ(peer.get_balance("USD"), price_t("50.0"));
}

TEST(PeerTest, UpdateBalances) {
  Peer peer(1, "test_user");
  peer.updateBalances("USD", price_t("100.5"), "RUB", price_t("5000.0"));
  ASSERT_EQ(peer.get_balance("USD"), price_t("100.5"));
  ASSERT_EQ(peer.get_balance("RUB"), price_t("-5000.0"));
}
