#include "input.hpp"

namespace in {
void take_int_input(int &input, const std::string &message) {
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
} // namespace in
