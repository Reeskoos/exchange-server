#include <gtest/gtest.h>

#include "core.hpp"
#include "peer.hpp"
#include "trade.hpp"

class TradeTest : public ::testing::Test {
 protected:
  Core core;
  std::shared_ptr<Peer> buyer1;
  std::shared_ptr<Peer> buyer2;
  std::shared_ptr<Peer> buyer3;
  std::shared_ptr<Peer> seller1;
  std::shared_ptr<Peer> seller2;
  std::shared_ptr<Peer> seller3;
  Trade* trade;

  void SetUp() override {
    buyer1 = std::make_shared<Peer>(1, "buyer1");
    buyer2 = std::make_shared<Peer>(2, "buyer2");
    buyer3 = std::make_shared<Peer>(3, "buyer3");
    seller1 = std::make_shared<Peer>(4, "seller1");
    seller2 = std::make_shared<Peer>(5, "seller2");
    seller3 = std::make_shared<Peer>(6, "seller3");
    core.AddPeer(buyer1->get_id(), buyer1);
    core.AddPeer(buyer2->get_id(), buyer2);
    core.AddPeer(buyer3->get_id(), buyer3);
    core.AddPeer(seller1->get_id(), seller1);
    core.AddPeer(seller2->get_id(), seller2);
    core.AddPeer(seller3->get_id(), seller3);
    trade = &core.getTradeModule({"USD", "RUB"});
  }

  void TearDown() override {
    core.RemovePeer(buyer1->get_id());
    core.RemovePeer(buyer2->get_id());
    core.RemovePeer(buyer3->get_id());
    core.RemovePeer(seller1->get_id());
    core.RemovePeer(seller2->get_id());
    core.RemovePeer(seller3->get_id());
  }
};

TEST_F(TradeTest, NoMatchOrder) {
  trade->AddOrder(buyer1->get_id(), 100, "75.0", Side::BUY, {"USD", "RUB"});
  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 0);
  EXPECT_EQ(buyer1->get_balance("RUB"), 0);
  EXPECT_EQ(seller1->get_balance("USD"), 0);
  EXPECT_EQ(seller1->get_balance("RUB"), 0);
}

TEST_F(TradeTest, PartialExecution) {
  trade->AddOrder(seller1->get_id(), 50, "75.0", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(buyer1->get_id(), 100, "75.0", Side::BUY, {"USD", "RUB"});
  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 50);
  EXPECT_EQ(buyer1->get_balance("RUB"), -3750);
  EXPECT_EQ(seller1->get_balance("USD"), -50);
  EXPECT_EQ(seller1->get_balance("RUB"), 3750);
}

TEST_F(TradeTest, MultipleOrderExecution) {
  trade->AddOrder(seller1->get_id(), 50, "74.56", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(seller1->get_id(), 50, "74.44", Side::SELL, {"USD", "RUB"});

  trade->AddOrder(buyer1->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});
  trade->AddOrder(buyer1->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});

  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 100);
  EXPECT_EQ(buyer1->get_balance("RUB"), -7450);

  EXPECT_EQ(seller1->get_balance("USD"), -100);
  EXPECT_EQ(seller1->get_balance("RUB"), 7450);
}

TEST_F(TradeTest, MultipleBuyersSingleSeller) {
  trade->AddOrder(seller1->get_id(), 100, "75.0", Side::SELL, {"USD", "RUB"});

  trade->AddOrder(buyer1->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});
  trade->AddOrder(buyer2->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});

  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 50);
  EXPECT_EQ(buyer1->get_balance("RUB"), -3750);
  EXPECT_EQ(buyer2->get_balance("USD"), 50);
  EXPECT_EQ(buyer2->get_balance("RUB"), -3750);

  EXPECT_EQ(seller1->get_balance("USD"), -100);
  EXPECT_EQ(seller1->get_balance("RUB"), 7500);
}

TEST_F(TradeTest, SingleBuyerMultipleSellers) {
  trade->AddOrder(buyer1->get_id(), 100, "75.0", Side::BUY, {"USD", "RUB"});

  trade->AddOrder(seller1->get_id(), 50, "75.0", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(seller2->get_id(), 50, "75.0", Side::SELL, {"USD", "RUB"});

  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 100);
  EXPECT_EQ(buyer1->get_balance("RUB"), -7500);
  EXPECT_EQ(seller1->get_balance("USD"), -50);
  EXPECT_EQ(seller1->get_balance("RUB"), 3750);
  EXPECT_EQ(seller2->get_balance("USD"), -50);
  EXPECT_EQ(seller2->get_balance("RUB"), 3750);
}

TEST_F(TradeTest, MultipleBuyersAndSellers) {
  trade->AddOrder(seller1->get_id(), 50, "74.56", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(seller2->get_id(), 50, "74.44", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(seller1->get_id(), 50, "74.50", Side::SELL, {"USD", "RUB"});

  trade->AddOrder(buyer1->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});
  trade->AddOrder(buyer2->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});
  trade->AddOrder(buyer1->get_id(), 50, "75.0", Side::BUY, {"USD", "RUB"});

  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 100);
  EXPECT_EQ(buyer1->get_balance("RUB"), -7450);  

  
  EXPECT_EQ(buyer2->get_balance("USD"), 50);
  EXPECT_EQ(buyer2->get_balance("RUB"), -3725);  

  
  EXPECT_EQ(seller1->get_balance("USD"), -100);
  EXPECT_EQ(seller1->get_balance("RUB"), 7453);  


  EXPECT_EQ(seller2->get_balance("USD"), -50);
  EXPECT_EQ(seller2->get_balance("RUB"), 3722); 
}

TEST_F(TradeTest, PriorityOrderExecution) {
  
  trade->AddOrder(buyer1->get_id(), 50, "56.54", Side::BUY, {"USD", "RUB"});
  trade->AddOrder(buyer2->get_id(), 50, "56.55", Side::BUY, {"USD", "RUB"});
  trade->AddOrder(buyer3->get_id(), 50, "56.56", Side::BUY, {"USD", "RUB"});

  
  trade->AddOrder(seller1->get_id(), 50, "56.50", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(seller2->get_id(), 50, "56.49", Side::SELL, {"USD", "RUB"});
  trade->AddOrder(seller3->get_id(), 50, "56.48", Side::SELL, {"USD", "RUB"});

  trade->MatchOrders();

  EXPECT_EQ(buyer1->get_balance("USD"), 50);
  EXPECT_EQ(buyer1->get_balance("RUB"), -2827);  

  EXPECT_EQ(buyer2->get_balance("USD"), 50);
  EXPECT_EQ(buyer2->get_balance("RUB"), -2827.5);  

  EXPECT_EQ(buyer3->get_balance("USD"), 50);
  EXPECT_EQ(buyer3->get_balance("RUB"), -2828);  

  EXPECT_EQ(seller1->get_balance("USD"), -50);
  EXPECT_EQ(seller1->get_balance("RUB"), 2827);  

  EXPECT_EQ(seller2->get_balance("USD"), -50);
  EXPECT_EQ(seller2->get_balance("RUB"), 2827.5);  

  EXPECT_EQ(seller3->get_balance("USD"), -50);
  EXPECT_EQ(seller3->get_balance("RUB"), 2828);
}
