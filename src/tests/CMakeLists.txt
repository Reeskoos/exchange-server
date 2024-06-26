find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_all
    client/client_test.cpp
    server/core_test.cpp
    server/order_test.cpp
    server/peer_test.cpp
    server/trade_test.cpp
    server/server_test.cpp
    server/session_test.cpp
    server/handler_test.cpp
    tests_main.cpp
)

target_link_libraries(test_all
    GTest::gtest
    GTest::gtest_main
    server
    client
    common
    Boost::system
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

include(GoogleTest)
gtest_discover_tests(test_all)
