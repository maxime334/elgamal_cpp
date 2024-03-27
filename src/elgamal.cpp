#include "elgamal.hpp"
#include "input.hpp"
#include "modulo.hpp"

Elgamal::Elgamal(std::uint8_t no_bits) {

  _prime = mod::prime_gen(no_bits);
  _generator = mod::find_primit_root(_prime);

  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::uint32_t> distr(2, _prime - 1);
  _alice_private = distr(gen); // Random value b/w 2 and prime-1.

  _public_key = mod::mod_exp(_generator, _alice_private, _prime);
}

void Elgamal::encrypt_message(std::vector<std::uint32_t> message) {
  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::uint32_t> distr(2, _prime - 1);

  // Bob generates a new private key for each message sent.
  _ephemeral_key = distr(gen);

  // Hence shared secret must change every time.
  _shared_secret = mod::mod_exp(_public_key, _ephemeral_key, _prime);

  _ciphertext1 = mod::mod_exp(_generator, _ephemeral_key, _prime);

  // Message is encrypted and stored.
  _ciphertext2 = std::move(message);
  for (std::uint32_t &i : _ciphertext2) {
    std::uint64_t val =
        (i * _shared_secret) & _prime; // Need a 64-bit before applying mod.
    i = val;
  }
}

std::string decrypt_message(std::vector<std::uint32_t> cipher,
                            std::uint32_t shared_secret, std::uint32_t prime) {
  std::uint32_t inv_shared_secret = mod::find_mod_inverse(shared_secret, prime);
  for (auto &i : cipher) {
    std::uint64_t chr =
        (i * inv_shared_secret) % prime; // 64-bit needed to contain overflow.
    i = chr;
  }
  return std::string(cipher.begin(), cipher.end());
}

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

  // Message ready to be encrypted through an 'array' of numbers.
  std::vector<std::uint32_t> enc_message(message.begin(), message.end());

  Elgamal elgamal(no_bits);
  std::cout << "Generated Prime: " << elgamal._prime << std::endl;
  std::cout << "Generator: " << elgamal._generator << std::endl;
  std::cout << "Alice's private key: " << elgamal._alice_private << std::endl;
  std::cout << "Alice publishes public key: " << elgamal._public_key
            << std::endl
            << " as well as the generated prime, and the generator of the "
               "cyclic group."
            << std::endl;

  elgamal.encrypt_message(enc_message); // Encryption stored in obj.

  std::cout << "Bob generates the ephemeral key: " << elgamal._ephemeral_key
            << std::endl;
  std::cout << "Alice receives the ciphertexts c1: " << elgamal._ciphertext1
            << " and c2: placeholder." << std::endl;
  std::cout << "Alice uses ciphertext c1 to gain the shared secret: "
            << elgamal._shared_secret << std::endl;
  std::cout << "Alice decrypts the ciphertext c2 and gains the message: "
            << decrypt_message(elgamal._ciphertext2, elgamal._shared_secret,
                               elgamal._prime);
}
