cmake_minimum_required(VERSION 3.10)
project(ExchangeServer)

set(CMAKE_CXX_STANDARD 17)

# Automatically download and bootstrap vcpkg if not found
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


find_package(Boost 1.65 REQUIRED system)
find_package(nlohmann_json REQUIRED)

add_subdirectory(src/common)
add_subdirectory(src/server)
add_subdirectory(src/client)
# add_subdirectory(tests)
