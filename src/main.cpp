#include "elgamal.hpp"
#include "modulo.hpp"

int main() {

  auto f = mod::find_primit_root(32251);

  elgamal_demo();

  //
  return 0;
}