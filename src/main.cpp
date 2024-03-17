#include <iostream>
#include <vector>
#include <fstream>

#include "rmq.hpp"

namespace {

  auto get_data(std::istream &is) { 
    std::vector<int64_t> array;
    std::vector<std::pair<std::size_t, std::size_t>> queries;

    char atribute = 0;
    int64_t arr_elem = 0;
    std::size_t l = 0, r = 0;
    for (; is.good(); ) {
      is >> atribute;
      if (is.fail()) {
        break;
      }
      if (atribute == 'k') {
        is >> arr_elem;
        array.push_back(arr_elem);
      } else if (atribute == 'q') {
        is >> l >> r;
        queries.push_back({l ,r});
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

