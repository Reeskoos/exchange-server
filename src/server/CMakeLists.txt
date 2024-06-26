# Source files for the server
set(SERVER_SOURCES
    core/core.cpp
    core/server.cpp
    core/session.cpp
    core/handler.cpp
    trade/trade.cpp
    trade/order.cpp
    users/peer.cpp
)

add_library(server
    ${SERVER_SOURCES}
)


target_include_directories(server PUBLIC include)

# Линкуем зависимости для библиотеки server
target_link_libraries(server PUBLIC Boost::system common nlohmann_json::nlohmann_json spdlog::spdlog)


add_executable(server_app main.cpp)

target_include_directories(server_app PUBLIC include)


target_link_libraries(server_app server)
