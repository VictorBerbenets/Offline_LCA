#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include <vector>

#include "rmq.hpp"

namespace {

  auto dice(int min, int max) {
    static std::uniform_int_distribution<int> distr(min, max);
    static std::random_device device;
    static std::mt19937 engine {device()};
    return distr(engine);
  }

} // <--- namespace

using namespace yLAB;

TEST(RMQ, RMQ1) {
  RmqSolver rmq_solver {1};
  ASSERT_EQ(rmq_solver.ans_query({0, 0}), 1);
}

TEST(RMQ, RMQ2) {
  RmqSolver rmq_solver {1, 1};
  ASSERT_EQ(rmq_solver.ans_query({0, 1}), 1);
}

TEST(RMQ, RMQ3) {
  RmqSolver rmq_solver {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  ASSERT_EQ(rmq_solver.ans_query({0, 11}), 1);
}

TEST(RMQ, RMQ4) {
  RmqSolver rmq_solver {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1};
  ASSERT_EQ(rmq_solver.ans_query({0, 11}), -1);
}

TEST(RMQ, RMQ5) {
  RmqSolver rmq_solver {-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  ASSERT_EQ(rmq_solver.ans_query({0, 11}), -1);
}

TEST(RMQ, RMQ6) {
  static constexpr int Size = 1000;

  std::vector<int> v(Size);
  std::iota(v.begin(), v.end(), -50);
  RmqSolver rmq_solver(v.begin(), v.end());
  for (int i = 0; i < Size; ++i) {
    for (int j = i; j < Size; ++j) {
      ASSERT_EQ(rmq_solver.ans_query({i, j}), *std::min_element(std::addressof(v[i]),
                                                                std::addressof(v[j + 1])));
    }
  }
}

TEST(RMQ, RMQ7) {
  static constexpr int Size = 1000;
  std::vector<int> v(Size);
  // random numbers
  std::generate(v.begin(), v.end(), [] { return dice(-100000, 100000); });
  RmqSolver rmq_solver(v.begin(), v.end());
  for (int i = 0; i < Size; ++i) {
    for (int j = i; j < Size; ++j) {
      ASSERT_EQ(rmq_solver.ans_query({i, j}), *std::min_element(std::addressof(v[i]),
                                                                std::addressof(v[j + 1])));
    }
  }
}

