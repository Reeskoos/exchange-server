#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core.hpp"
#include "trade.hpp"
#include "peer.hpp"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class TradeTest : public ::testing::Test {
protected:
    Core core;
    std::shared_ptr<Peer> buyer;
    std::shared_ptr<Peer> seller;
    Trade* trade;

    void SetUp() override {
        buyer = std::make_shared<Peer>(1, "buyer");
        seller = std::make_shared<Peer>(2, "seller");
        core.AddPeer(buyer->get_id(), buyer);
        core.AddPeer(seller->get_id(), seller);
        trade = &core.getTradeModule({"USD", "RUB"});
    }

    void TearDown() override {
        core.RemovePeer(buyer->get_id());
        core.RemovePeer(seller->get_id());
    }
};

TEST_F(TradeTest, NoMatchOrder) {
    trade->AddOrder(buyer->get_id(), 100, "75.0", Side::BUY, {"USD", "RUB"});
    trade->MatchOrders();

    EXPECT_EQ(buyer->get_balance("USD"), 0);
    EXPECT_EQ(buyer->get_balance("RUB"), 0);
    EXPECT_EQ(seller->get_balance("USD"), 0);
    EXPECT_EQ(seller->get_balance("RUB"), 0);
}

TEST_F(TradeTest, PartialExecution) {
    trade->AddOrder(seller->get_id(), 50, "75.0", Side::SELL, {"USD", "RUB"});
    trade->AddOrder(buyer->get_id(), 100, "75.0", Side::BUY, {"USD", "RUB"});
    trade->MatchOrders();

    EXPECT_EQ(buyer->get_balance("USD"), 50);
    EXPECT_EQ(buyer->get_balance("RUB"), -3750); // 50 * 75
    EXPECT_EQ(seller->get_balance("USD"), -50);
    EXPECT_EQ(seller->get_balance("RUB"), 3750);
}

TEST_F(TradeTest, MultipleOrderExecution) {
    trade->AddOrder(seller->get_id(), 50, "74.56", Side::SELL, {"USD", "RUB"});
    trade->AddOrder(seller->get_id(), 50, "74.44", Side::SELL, {"USD", "RUB"});

    trade->AddOrder(buyer->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});
    trade->AddOrder(buyer->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});

    trade->MatchOrders();

    EXPECT_EQ(buyer->get_balance("USD"), 100);
    EXPECT_EQ(buyer->get_balance("RUB"), -7500); 
    EXPECT_EQ(seller->get_balance("USD"), -100);
    EXPECT_EQ(seller->get_balance("RUB"), 7500);
}
