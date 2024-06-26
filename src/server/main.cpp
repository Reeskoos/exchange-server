
#include "server.hpp"
#include "spdlog/spdlog.h"

int main() {
  try {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Starting server...");

    boost::asio::io_service io_service;

    Server s(io_service);
    s.run();

    spdlog::info("Server running on port {}", port);

    io_service.run();

  } catch (std::exception& e) {
    spdlog::error("Exception: {}", e.what());
  }

  return 0;
}
