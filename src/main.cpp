#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

#include "rmq.hpp"

namespace {

  void check_input(std::istream &is) {
    if (is.fail()) {
      throw std::runtime_error {"data reading error"};
    }
  }

  auto get_data(std::istream &is) { 
    std::vector<int64_t> array;
    std::vector<std::pair<std::size_t, std::size_t>> queries;

    char atribute = 0;
    int64_t arr_elem = 0;
    std::size_t l = 0, r = 0;
    for (; is.good(); ) {
      is >> atribute;
      if (is.fail()) { break; }

      if (atribute == 'k') {
        is >> arr_elem;
        check_input(is);
        array.push_back(arr_elem);
      } else if (atribute == 'q') {
        is >> l >> r;
        check_input(is);
        queries.push_back({l ,r});
      }
      else {
        throw std::runtime_error {"invalid input atribute, expected k or q, got: " +
                                  std::to_string(atribute)};
      }
    }
    return std::make_pair(array, queries);
  }

} // <--- namespace

int main() {
  auto [array, queries] = get_data(std::cin);
  yLAB::RmqSolver<int64_t> rmq(array.begin(), array.end());

  for (auto &&[l, r] : queries) {
    std::cout << rmq.ans_query({l, r}) << ' ';
  }
  std::cout << std::endl;
}

