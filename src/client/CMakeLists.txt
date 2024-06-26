# Source files for the client
set(CLIENT_SOURCES
    src/client.cpp
    main.cpp
)

add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE Boost::system Boost::multiprecision nlohmann_json::nlohmann_json common)
