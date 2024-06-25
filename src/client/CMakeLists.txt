project(Client)

add_executable(client
    client.cpp
)

target_include_directories(client PUBLIC include ../common/include)

target_link_libraries(client PRIVATE Boost::system nlohmann_json)
