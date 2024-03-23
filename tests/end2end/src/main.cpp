#include <iostream>
#include <stdexcept>
#include <string>

#include "end2end.hpp"

namespace ts = testing;

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 6) {
    throw std::runtime_error {"invalid set of arguments, expected 1 - 5, got" +
                              std::to_string(argc - 1)};
  }

  auto test_num = std::stoul(argv[1]);
  ts::generator<int> gen(test_num);
  if (argc == 2) {
    gen.generate_tests();
  } else if (argc == 3) {
    gen.generate_tests(std::stol(argv[2]));
  } else if (argc == 4) {
    gen.generate_tests(std::stol(argv[2]), std::stol(argv[3]));

  } else if (argc == 5) {
    gen.generate_tests(std::stol(argv[2]), std::stol(argv[3]),
                       std::stoul(argv[4]));

  } else if (argc == 6) {
    gen.generate_tests(std::stol(argv[2]), std::stol(argv[3]),
                       std::stoul(argv[4]), std::stoul(argv[5]));

  }
}

