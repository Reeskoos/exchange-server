#include <gtest/gtest.h>

#include "order.hpp"

TEST(OrderTest, CreateOrder) {
  Order order(1, 100, "50.5", Side::BUY, {"USD", "RUB"});
  ASSERT_EQ(order.getClientId(), 1);
  ASSERT_EQ(order.getVolume(), 100);
  ASSERT_EQ(order.getPrice(), price_t("50.5"));
  ASSERT_EQ(order.getSide(), Side::BUY);
  ASSERT_EQ(order.getCurrencyPair(), CurrencyPair("USD", "RUB"));
}

TEST(OrderTest, CompareOrders) {
  Order order1(1, 100, "50.5", Side::BUY, {"USD", "RUB"});
  Order order2(1, 100, "50.5", Side::BUY, {"USD", "RUB"});

  ASSERT_TRUE(order1 > order2);

  Order order3(1, 100, "50.1", Side::SELL, {"USD", "RUB"});
  Order order4(1, 100, "50.3", Side::SELL, {"USD", "RUB"});

  ASSERT_TRUE(order3 < order4);

  Order order5(1, 100, "50.3", Side::SELL, {"USD", "RUB"});
  Order order6(1, 100, "50.5", Side::BUY, {"USD", "RUB"});

  ASSERT_TRUE(order5 > order3);
  ASSERT_FALSE(order6 > order1);
}

TEST(OrderTest, ReduceVolume) {
  Order order(1, 100, "50.5", Side::BUY, {"USD", "RUB"});

  order.reduceVolume(50);
  ASSERT_EQ(order.getVolume(), 50);

  order.reduceVolume(25);
  ASSERT_EQ(order.getVolume(), 25);

  EXPECT_ANY_THROW(order.reduceVolume(30));
}