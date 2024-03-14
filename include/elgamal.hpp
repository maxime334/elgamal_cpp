#pragma once
#include "input.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

struct Elgamal {
  // Large prime number.
  std::uint32_t _prime{0};
  // Generator of multiplicative group Zp*..
  std::uint16_t _generator{0};

  std::uint32_t _bob_private{0};

  std::uint32_t _alice_private{0};

  std::uint32_t _public_key{0};

  std::uint32_t _ephemeral_key{0};

  Elgamal() = delete;

  // Generates both public and private key.
  Elgamal(std::uint8_t bit);
};

/**
  @brief Creates a byte array of the message.
*/
char *byte_array(std::string &message) noexcept;

/**
  Simple representation of the Elgamal encryption for the user.
*/
void elgamal_demo();
