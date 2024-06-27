#include <iostream>

#include "client.hpp"
#include "common.hpp"

int main() {
  try {
    Client client("127.0.0.1", port);
    client.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
