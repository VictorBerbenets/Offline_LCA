#include <iostream>
#include <vector>
#include <fstream>

#include "rmq.hpp"

namespace {

  bool compare_answers(std::ifstream &test_file, std::ifstream &ans_file) { 
    std::vector<int64_t> array;
    std::vector<std::pair<std::size_t, std::size_t>> queries;

    char atribute = 0;
    int64_t arr_elem = 0;
    std::size_t l = 0, r = 0;
    for (; test_file.good(); ) {
      test_file >> atribute;
      if (atribute == 'k') {
        test_file >> arr_elem;
        array.push_back(arr_elem);
      } else if (atribute == 'q') {
        test_file >> l >> r;
        queries.push_back({l ,r});
      }
    }
    yLAB::RmqSolver rmq(array.begin(), array.end());
    int64_t answer = 0;
    for (auto [l, r] : queries) {
      ans_file >> answer;
      if (answer != rmq.ans_query({l, r})) {
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

