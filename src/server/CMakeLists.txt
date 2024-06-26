# Source files for the server
set(SERVER_SOURCES
    core/core.cpp
    core/server.cpp
    core/session.cpp
    core/handler.cpp
    trade/trade.cpp
    trade/order.cpp
    users/peer.cpp
    main.cpp
)

add_executable(server ${SERVER_SOURCES})
target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE Boost::system Boost::multiprecision nlohmann_json::nlohmann_json spdlog::spdlog common)
