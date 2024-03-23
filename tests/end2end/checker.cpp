#include <iostream>
#include <vector>
#include <fstream>

#include "rmq.hpp"

namespace {

  bool compare_answers(std::ifstream &test_file, std::ifstream &ans_file) { 
    std::vector<int64_t> array;
    std::vector<int64_t> queries;
    std::vector<int64_t> data;
    
    std::copy(std::istream_iterator<int64_t>(test_file), std::istream_iterator<int64_t>(),
              std::back_inserter(data));
    
    auto begin = data.begin();
    std::copy_n(std::next(begin), *begin, std::back_inserter(array));
    std::advance(begin, *begin + 1);
    std::copy_n(std::next(begin), *begin * 2, std::back_inserter(queries));
 
    yLAB::RmqSolver rmq(array.begin(), array.end());
    int64_t answer = 0;
    for (std::size_t i = 0, size = queries.size(); i < size; i += 2) {
      ans_file >> answer;
      if (answer != rmq.ans_query({queries[i], queries[i + 1]})) {
        return false;
      }
    }
    return true;
  }

} // <--- namespace

int main(int argc, char **argv) {
  std::ifstream test_file {argv[1]};
  std::ifstream ans_file {argv[2]};

  if (compare_answers(test_file, ans_file)) {
    std::cout << "passed" << std::endl;
  } else {
    std::cout << "not passed" << std::endl;
  }
}

