#include "modulo.hpp"
#include <exception>
#include <random>
#include <stdexcept>

namespace mod {

inline std::uint64_t gcd(std::uint64_t a, std::uint64_t b) {
  return b == 0 ? a : gcd(b, a % b);
}

std::uint32_t mod_exp(std::uint64_t base, std::uint64_t exp, std::uint32_t p) {

  if (exp == 0)
    return 1;
  if (exp == 1)
    return base % p;

  std::uint64_t result{0};

  // Even exponent.
  if (exp % 2 == 0) {
    result = mod_exp(base, exp / 2, p);
    if (result >= std::pow(2, 32))
      throw new std::overflow_error(
          "Overflow occured during modular exponentation.");
    // (2^32)^2 cannot be contained in 64-bit unsigned.
    result = (result * result) % p;
  }
  // Odd exponent.
  else {
    result = mod_exp(base, exp - 1, p);
    result = (base * result) % p;
  }

  return result;
}

bool is_prime(std::uint32_t prime) noexcept {
  // https://cp-algorithms.com/algebra/primality_tests.html.

  /**
    @brief Check compositeness of a^(d)mod(n).
    @return False if composite.
  */
  static auto check_composite = [](std::uint64_t n, int a, std::uint64_t d,
                                   int s) -> bool {
    auto admodn = mod_exp(a, d, n);

    // Congruent to 1mod(n) or -1mod(n).
    if (admodn == 1 || admodn == (n - 1))
      return false;

    // Checking all values a^(2^(r)d).
    for (int r = 1; r < s; r++) {
      admodn = mod_exp(admodn, 2, n);
      // Congruent to -1mod(n).
      if (admodn == (n - 1))
        return false;
    }

    return true;
  };

  // For a 32-bit integer, enough to check the first 4 prime bases.
  static constexpr std::array<int, 4> bases = {2, 3, 5, 7};

  // For a 64-bit integer, it is enough to check the first 12 prime bases.
  // https://oeis.org/A014233.
  // Not used yet.
  // static constexpr std::array<int, 12> bases_64 = {2,  3,  5,  7,  11, 13,
  // 17, 19, 23, 29, 31, 37};

  if (prime < 4)
    return (prime == 2) || (prime == 3); // One not considered a prime.

  // Find s,d such that (prime-1) = 2^(s)*d.
  int s = 0;
  std::uint64_t d = (prime - 1);
  // While d is even, divide by 2.
  while ((d & 1) == 0) {
    d >>= 1; // Divide by 2. Bitwise operation.
    s++;     // Increment s, for 2^s.
  }

  for (const int &base : bases) {
    if (prime == base)
      return true;
    if (check_composite(prime, base, d, s))
      return false;
  }
  return true;
}

std::uint32_t prime_gen(std::uint8_t bit_length) {
  if (bit_length >= 32)
    throw new std::overflow_error("Bit-length too high. Overflow will occur.");

  // Min-max value range.
  const std::uint32_t min_val = std::pow(2, bit_length - 1) + 1;
  const std::uint32_t max_val = std::pow(2, bit_length);

  static std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::uint32_t> distr(min_val, max_val);

  std::uint32_t number{0};

  // Keep drawing numbers untils it's prime.
  do {
    number = distr(gen);
  } while (!is_prime(number));

  return number;
}

std::uint32_t pollards_rho(std::uint32_t value) {
  // https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm
  // Pollard's Rho seems to fail for prime powers:
  // https://stackoverflow.com/questions/33528953/pollard-rho-cant-find-factor

  // Simple polynomial modulo (value).
  static const auto gx = [value](std::uint32_t x, std::uint32_t b) {
    return ((x * x) + b) % value;
  };

  std::uint32_t factor{0};

  for (int i = 0; i < value; i++) {
    // Trying to find a proper non-trivial prime factor, with different
    // starting i values.
    long long x{i};
    long long y{x};
    factor = 1;

    // Algo fails to find prime factorization of 8 and 16. Brute force return.
    if (value == 4 || value == 8)
      return 2;

    while (factor == 1) {
      x = gx(x, 1);
      y = gx(gx(y, 1), 1);
      factor = gcd(std::abs(x - y), value); // Cycle has been found.
    }

    // Wrong starting value, we must try something else.
    if (factor == value)
      continue;

    /*
      Algo finds a non-trivial factor, which may not be prime.
      Modification to standard Pollard's Rho algo.
    */
    if (!is_prime(factor)) {
      value = factor; // Find a new factorization over non-prime factor.
      i = 0;          // Restart loop.
      continue;
    }

    break; // Non-trivial prime factor found.
  }

  return factor;
}

std::vector<std::uint32_t> find_prime_factors(std::uint32_t value) {

  // Begin w/ two factors for value.
  std::uint32_t p{0};
  std::uint32_t q{0};
  std::vector<std::uint32_t> factors;

  if (is_prime(value))
    return {value}; // Input is already a prime.

  /**
    @brief Inserts only prime factor if not already found.
  */
  static const auto unique_insert =
      [](std::uint32_t prime, std::vector<std::uint32_t> &factors) -> void {
    if (std::find(factors.begin(), factors.end(), prime) == factors.end())
      factors.push_back(prime); // Not found, can append to vector.
    // Else, been found, no insert.
  };

  // q factorized until it's a prime.
  do {
    p = pollards_rho(value);
    unique_insert(p, factors);
    q = value / p;
    value = q; // Next loop will find prime factor of q.
  } while (!is_prime(q));
  unique_insert(q, factors);

  return factors;
}

std::uint32_t find_primit_root(std::uint32_t prime) {
  if (!is_prime(prime))
    throw new std::invalid_argument("find_primit_root() can only find the "
                                    "primitive element of a prime.");

  std::uint32_t s = (prime - 1); // Euler's Totient Function.

  auto prime_factors = find_prime_factors(s);
  auto skip_outer = false;

  // Testing multiple values for generator g.
  for (int g = 2; g < prime; g++) {
    skip_outer = false;

    for (const auto pi : prime_factors) {
      int exp = (prime - 1) / pi;
      if (mod_exp(g, exp, prime) == 1) {
        // Try another value for g.
        skip_outer = true;
        break;
      }
    }
    // Generator has been found iff mod_exp != 1.
    if (skip_outer)
      continue;
    else
      return g;
  }
  // Not found a generator.
  return 0;
}
} // namespace mod