#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <random>

namespace mod {
/**
    Computes gcd(a, b).
*/
inline std::uint64_t gcd(std::uint64_t a, std::uint64_t b);

/**
    @brief Computes base^exp(mod p).
    Fast modular positive exponentiation implemented via square & multiply
   algorithm.
*/
std::uint32_t mod_exp(std::uint64_t base, std::uint64_t exp, std::uint32_t p);

/**
    @brief Deterministic Rabin-Miller primality test which verifies prime up to
   2^32.
*/
bool is_prime(const std::uint32_t prime) noexcept;

/**
    @brief Generate a prime number given the bit length. Accepts prime of 31
   bits and lower only, else overflow occurs.
*/
std::uint32_t prime_gen(std::uint8_t bit_length);

/**
    @brief Computes and returns a non-trival prime factor of the argument input.
*/
std::uint32_t pollards_rho(std::uint32_t value);

/**
    @brief Computes and returns the unique prime factors of the argument input.
*/
std::vector<std::uint32_t> find_prime_factors(std::uint32_t value);

/**
    @brief Finds smallest primitive root of the prime input.
*/
std::uint32_t find_primit_root(std::uint32_t prime);
} // namespace mod