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
