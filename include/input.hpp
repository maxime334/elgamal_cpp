#pragma once
#include <iostream>
#include <limits>
#include <string>

namespace in {
/**
  @brief Tells the user a message, and waits for an integer input from the user.
*/
void take_int_input(int &input, const std::string &message) noexcept;

/**
  @brief Takes a string input from the user, given a max_length.
*/
void take_string_input(std::string &input, const std::string &message,
                       int max_length) noexcept;
} // namespace in
