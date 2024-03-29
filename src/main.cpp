#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>

#include "rmq.hpp"

namespace {

  auto get_data(std::istream &is) { 
    std::vector<int> array;
    std::vector<int> queries;
    std::vector<int> data;
    
    std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(),
              std::back_inserter(data));
    
    auto begin = data.begin();
    std::copy_n(std::next(begin), *begin, std::back_inserter(array));
    std::advance(begin, *begin + 1);
    std::copy_n(std::next(begin), *begin * 2, std::back_inserter(queries));
 
    return std::make_pair(array, queries);
  }

} // <--- namespace

int main() {
  auto [array, queries] = get_data(std::cin);
  yLAB::RmqSolver rmq(array.begin(), array.end());

  for (std::size_t i = 0, size = queries.size(); i < size; i += 2) {
    std::cout << rmq.ans_query({queries[i], queries[i + 1]}) << ' ';
  }
  std::cout << std::endl;
}

