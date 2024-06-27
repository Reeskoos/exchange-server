add_library(client
    src/client.cpp
)

target_include_directories(client PUBLIC include)
target_link_libraries(client PUBLIC Boost::system common nlohmann_json::nlohmann_json spdlog::spdlog)

add_executable(client_app main.cpp)

target_link_libraries(client_app client)
target_include_directories(client_app PUBLIC include)
