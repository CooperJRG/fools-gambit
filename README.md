# fools-gambit
## 0. Project Philosophy
This engine will be built first and foremost as a learning project for myself. LLMs will not be used for the creation of this engine, and the code is meant to be understandable, clear, and not obfuscated.
The engine will be iterated upon as a sequence of measurable versions.
The core idea is that:
- Every major feature will be:
  1. Implemented in a way that is clear and understandable, with comments and documentation.
  2. Tested with a suite of unit tests to ensure correctness.
  3. Benchmarked against a set of standard chess positions and scenarios to measure performance and strength.
  4. Matched against the previous version to ensure that the new version is an improvement in terms of strength and/or performance.
  5. Documented with a changelog that describes the changes made and the rationale behind them.

Fool’s Gambit is a C++ UCI-compatible chess engine implementing bitboard move
generation, alpha-beta search, transposition tables, SPRT-based strength testing,
and NNUE-style incrementally updated neural evaluation.

(To build in development mode, use the following commands:
cmake -B build -G ninja
cmake --build build)
