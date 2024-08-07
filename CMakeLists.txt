cmake_minimum_required(VERSION 3.10)
project(ExchangeServer)

set(CMAKE_CXX_STANDARD 17)

# Install Boost if not found
find_package(Boost 1.65 REQUIRED COMPONENTS system)
if (NOT Boost_FOUND)
    message(STATUS "Boost not found, installing...")
    execute_process(COMMAND sudo apt-get install -y libboost-all-dev)
    find_package(Boost REQUIRED COMPONENTS system)
endif()

# Install GTest if not found
find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
    message(STATUS "GTest not found, installing...")
    execute_process(COMMAND sudo apt-get install -y libgtest-dev)
    execute_process(COMMAND sudo cmake -S /usr/src/gtest -B /usr/src/gtest/build)
    execute_process(COMMAND sudo cmake --build /usr/src/gtest/build --target install)
    find_package(GTest REQUIRED)
endif()

# Use vcpkg to manage dependencies if available
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(NOT DEFINED ENV{VCPKG_ROOT})
        set(VCPKG_ROOT "${CMAKE_BINARY_DIR}/vcpkg")
        if(NOT EXISTS ${VCPKG_ROOT})
            message(STATUS "Downloading vcpkg...")
            execute_process(COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT})
            execute_process(COMMAND ${CMAKE_COMMAND} -E chdir ${VCPKG_ROOT} ./bootstrap-vcpkg.sh)
        endif()
        set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    else()
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    endif()
endif()

# Use vcpkg toolchain file if provided
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable not set and CMAKE_TOOLCHAIN_FILE not provided")
endif()

# Install other dependencies via vcpkg
execute_process(COMMAND ${VCPKG_ROOT}/vcpkg install nlohmann-json spdlog)

find_package(nlohmann_json REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Add subdirectories
add_subdirectory(src/common)
add_subdirectory(src/server)
add_subdirectory(src/client)

# Enable testing
enable_testing()
add_subdirectory(src/tests)
