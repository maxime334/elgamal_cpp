#include "elgamal.hpp"
#include "input.hpp"
#include "modulo.hpp"

char *byte_array(std::string &message) noexcept { return &message[0]; }

void elgamal_demo() {

  int no_bits;
  while (true) {
    in::take_int_input(
        no_bits, "A prime of at most 31 bits must be generated. Choose the "
                 "number of bits:");
    if (no_bits < 32)
      break;
    // Else.
    std::cout << "The number of bits is too high." << std::endl;
  }

  std::string message;
  in::take_string_input(
      message,
      "Please enter the encrypted message Bob is going to send to Alice.", 5);

  Elgamal elgamal(no_bits);
  std::cout << "Generated Prime: " << elgamal._prime << std::endl;
  std::cout << "Generator: " << elgamal._generator << std::endl;
  std::cout << "Alice's private key: " << elgamal._alice_private << std::endl;
  std::cout << "Bob's private key: " << elgamal._bob_private << std::endl;
  // std::cout << "Bob's private key: "<<
}

Elgamal::Elgamal(std::uint8_t no_bits) {

  _prime = mod::prime_gen(no_bits);
  _generator = mod::find_primit_root(_prime);

  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::uint32_t> distr(2, _prime - 1);
  _alice_private = distr(gen); // Random value b/w 2 and prime-1.

  _public_key = mod::mod_exp(_generator, _bob_private, _prime);

  _bob_private = distr(gen);
}