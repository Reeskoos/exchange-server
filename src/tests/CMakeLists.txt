project(Tests)

enable_testing()

add_executable(server_tests
    server_tests.cpp
)

add_executable(client_tests
    client_tests.cpp
)

target_link_libraries(server_tests PRIVATE server common)
target_link_libraries(client_tests PRIVATE client common)

add_test(NAME ServerTests COMMAND server_tests)
add_test(NAME ClientTests COMMAND client_tests)
