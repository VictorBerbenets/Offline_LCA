#include <gtest/gtest.h>
#include <iterator>
#include <ranges>
#include <random>
#include <algorithm>
#include <vector>
#include <set>

#include "cartesian_tree.hpp"

using namespace yLAB;

namespace {

auto dice(int min, int max) {
  static std::uniform_int_distribution<int> distr {min, max};
  static std::random_device device;
  static std::mt19937 engine {device()};
  return distr(engine);
}

} // <--- namespace

namespace rgs = std::ranges;

TEST(Iterator, PrevMethod1) {
  Treap tr {1};
  ASSERT_EQ(std::prev(tr.end())->second, 1);
}

TEST(Iterator, PrevMethod2) {
  std::set<int> set {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Treap tr(set.begin(), set.end());
  for (auto begin = tr.end(); auto &&v : set | std::views::reverse) {
    ASSERT_EQ((--begin)->second, v);
  }
}

TEST(Iterator, PrevMethod3) {
  std::set<int> set;
  for (int i = 0; i < 1000; ++i) {
    set.insert(i);
  }
  Treap<int> tr(set.begin(), set.end());
  for (auto begin = tr.end(); auto &&v : set | std::views::reverse) {
    ASSERT_EQ((--begin)->second, v);
  }
}

TEST(Iterator, NextMethod1) {
  Treap tr {1, 2};
  ASSERT_EQ(std::next(tr.begin())->second, 2);
}

TEST(Iterator, NextMethod2) {
  std::set<int> set {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Treap tr(set.begin(), set.end());
  auto begin = tr.cbegin();
  auto end = tr.cend();
  for (auto st_begin = set.cbegin(), st_end = set.cend(); st_begin != st_end; ++st_begin) {
    if (auto next = std::next(begin++); next != end) {
      ASSERT_EQ(next->second, *std::next(st_begin));
    }
  }
}

TEST(Iterator, NextMethod3) {
  std::set<int> set;
  for (int i = 0; i < 1000; ++i) {
    set.insert(i);
  }
  Treap tr(set.begin(), set.end());
  auto begin = tr.cbegin();
  auto end = tr.cend();
  for (auto st_begin = set.cbegin(), st_end = set.cend(); st_begin != st_end; ++st_begin) {
    if (auto next = std::next(begin++); next != end) {
      ASSERT_EQ(next->second, *std::next(st_begin));
    }
  }
}

TEST(Iterator, Distance1) {
  Treap<int> tr;
  ASSERT_EQ(std::distance(tr.cbegin(), tr.cend()), 0);
}

TEST(Iterator, Distance2) {
  static constexpr int Size = 1000;

  std::vector<int> stor(Size);
  std::iota(stor.begin(), stor.end(), 0);
  Treap tr(stor.begin(), stor.end());
  for (auto begin1 = tr.begin(), end = tr.end(); begin1 != end; ++begin1) {
    int distance = 0;
    for (auto begin2 = begin1; begin2 != end; ++begin2, ++distance) {
      ASSERT_EQ(std::distance(begin1, begin2), distance);
    }
  }
}

TEST(Iterator, Advance1) {
  Treap tr {1, 2};
  auto begin = tr.begin();
  std::advance(begin, 1);
  ASSERT_EQ(begin->second, 2);
}

TEST(Iterator, Advance2) {
  static constexpr int Size = 1000;

  std::vector<int> stor(Size);
  std::iota(stor.begin(), stor.end(), 0);
  Treap tr(stor.begin(), stor.end());
  for (int i = 0; i < Size; ++i) {
    auto tmp = tr.begin();
    std::advance(tmp, i);
    ASSERT_EQ(tmp->second, stor[i]);
  }
}

