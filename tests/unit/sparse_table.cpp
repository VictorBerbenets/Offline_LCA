#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>

#include "sparse_table.hpp"

using namespace yLAB;


namespace {

  auto dice(int min, int max) {
    static std::uniform_int_distribution<int> distr(min, max);
    static std::random_device device;
    static std::mt19937 engine {device()};
    return distr(engine);
  }

} // <--- namespace


TEST(SparseTable, Sparse1) {
  SparseTable sparse_t{1};
  ASSERT_EQ(sparse_t.min({0, 0}), 1);
}

TEST(SparseTable, Sparse2) {
  static constexpr int ArrSize = 1000;

  std::vector array(ArrSize, 0);
  std::generate(array.begin(), array.end(), [] { return dice(-1, 1000); });

  SparseTable sparse_t(array.cbegin(), array.cend(), array.size());
  for (int i = 0; i < ArrSize; ++i) {
    for (int j = i; j < ArrSize; ++j) {
      ASSERT_EQ(sparse_t.min({i, j}), *std::min_element(std::addressof(array[i]),
                                                        std::addressof(array[j + 1])));
    }
  }
}

TEST(SparseTable, Sparse3) {
  static constexpr int ArrSize = 1000;

  std::vector array(ArrSize, -1);
  SparseTable sparse_t(array.cbegin(), array.cend(), array.size());

  for (int i = 0; i < ArrSize; ++i) {
    for (int j = i; j < ArrSize; ++j) {
      ASSERT_EQ(sparse_t.min({i, j}), *std::min_element(std::addressof(array[i]),
                                                        std::addressof(array[j + 1])));
    }
  }
}

