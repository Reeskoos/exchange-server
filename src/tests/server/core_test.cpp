#include <gtest/gtest.h>
#include "core.hpp"

class CoreTest : public ::testing::Test {
protected:
    Core core;
};

TEST_F(CoreTest, RegisterNewUser) {
    std::string username = "test_user";
    std::size_t user_id = core.RegisterNewUser(username);
    
    ASSERT_EQ(core.GetUserName(user_id), username);
}

TEST_F(CoreTest, AddAndRemovePeer) {
    std::string username = "test_user";
    std::size_t user_id = core.RegisterNewUser(username);

    auto peer = std::make_shared<Peer>(user_id, username);
    core.AddPeer(user_id, peer);
    
    ASSERT_EQ(core.GetPeer(user_id)->get_username(), username);

    core.RemovePeer(user_id);
    ASSERT_THROW(core.GetPeer(user_id), std::runtime_error);
}

TEST_F(CoreTest, GetTradeModule) {
    CurrencyPair pair = std::make_pair("USD", "RUB");
    Trade& trade = core.getTradeModule(pair);
    
    ASSERT_EQ(trade.getCurrencyPair(), pair);
}

TEST_F(CoreTest, GetTradeModules) {
    const auto& trade_modules = core.getTradeModules();
    
    ASSERT_EQ(trade_modules.size(), 2);
    ASSERT_NE(trade_modules.find(std::make_pair("USD", "RUB")), trade_modules.end());
    ASSERT_NE(trade_modules.find(std::make_pair("RUB", "USD")), trade_modules.end());
}

TEST_F(CoreTest, GetUserName) {
    std::string username = "test_user";
    std::size_t user_id = core.RegisterNewUser(username);
    
    ASSERT_EQ(core.GetUserName(user_id), username);
    ASSERT_THROW(core.GetUserName(999), std::runtime_error); // Non-existent user
}

TEST_F(CoreTest, GetPeer) {
    std::string username = "test_user";
    std::size_t user_id = core.RegisterNewUser(username);

    auto peer = std::make_shared<Peer>(user_id, username);
    core.AddPeer(user_id, peer);
    
    ASSERT_EQ(core.GetPeer(user_id), peer);
    ASSERT_THROW(core.GetPeer(999), std::runtime_error); // Non-existent user
}

TEST_F(CoreTest, AddPeer) {
    std::string username = "test_user";
    std::size_t user_id = core.RegisterNewUser(username);

    auto peer = std::make_shared<Peer>(user_id, username);
    core.AddPeer(user_id, peer);
    
    ASSERT_EQ(core.GetPeer(user_id), peer);
}

TEST_F(CoreTest, RemovePeer) {
    std::string username = "test_user";
    std::size_t user_id = core.RegisterNewUser(username);

    auto peer = std::make_shared<Peer>(user_id, username);
    core.AddPeer(user_id, peer);
    core.RemovePeer(user_id);
    
    ASSERT_THROW(core.GetPeer(user_id), std::runtime_error);
}

