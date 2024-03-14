#include "elgamal.hpp"
#include "modulo.hpp"

void elgamal_demo() {

  int no_bits;
  while (true) {
    in::take_int_input(
        no_bits, "A prime of at most 32 bits must be generated. Choose the "
                 "number of bits:");
    if (no_bits < 32)
      break;
    // Else.
    std::cout << "The number of bits is too high." << std::endl;
  }

  Elgamal elgamal(no_bits);
}

Elgamal::Elgamal(std::uint8_t no_bits) {
  _prime = mod::prime_gen(no_bits);
  std::cout << _prime << std::endl;
  _generator = mod::find_primit_root(_prime);
  std::cout << _generator << std::endl;

  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::uint32_t> distr(2, _prime - 1);
  _bob_private = distr(gen); // Random value b/w 2 and prime-1.

  _public_key = mod::mod_exp(_generator, _bob_private, _prime);
}