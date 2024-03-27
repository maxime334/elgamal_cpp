# elgamal_cpp
Simple Elgamal encryption scheme in C++.
Elgamal proposes a public-key cryptosystem that is based on the Discrete Logarithm problem, that is computationally intractable in general.
The implementation does not work for more than 32-bit integers, to contain overflow.

A new ephemeral key, and hence shared secret is generated for each message sent by Bob to Alice.
