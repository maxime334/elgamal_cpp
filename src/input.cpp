#include "input.hpp"

namespace in {
void take_int_input(int &input, const std::string &message) noexcept {
  std::cout << message << std::endl;
  while (true) {
    std::cin >> input;
    if (!std::cin) {
      std::cout << "Please enter an integer. Enter again." << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    } else {
      break;
    }
  }
}

void take_string_input(std::string &input, const std::string &message,
                       int max_length) noexcept {
  std::cout << message << std::endl;
  while (true) {
    std::cin >> input;
    if (!std::cin || input.length() > max_length) {
      std::cout << "Please enter less than " << max_length << " characters."
                << std::endl;
      std::cin.clear();
      continue;
    } else {
      break;
    }
  }
}
} // namespace in
