project(Server)

add_executable(server
    server.cpp
)

target_include_directories(server PUBLIC include ../common/include)

target_link_libraries(server PRIVATE Boost::system nlohmann_json)
