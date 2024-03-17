#pragma once

#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <cmath>

#include "utils.hpp"

namespace yLAB {

template <typename T>
class SparseTable {
 public:
  using size_type   = std::size_t;
  using value_type  = T;
  using sparse_type = std::vector<std::vector<value_type>>;

  constexpr SparseTable() = default;

  constexpr SparseTable(std::initializer_list<value_type> i_list)
      : SparseTable(i_list.begin(), i_list.end(), i_list.size()) {}

  template <std::input_iterator Iter>
  constexpr SparseTable(Iter begin, Iter end, size_type n) {
    construct(begin, end, n);
  }

  virtual ~SparseTable() = default;

  constexpr value_type min(const std::pair<size_type, size_type> &query) const {
    int i = log2_floor(query.second - query.first + 1);

    return std::min(sparse_[i][query.first],
                    sparse_[i][query.second - (1 << i) + 1]);
  }

  template <std::input_iterator Iter>
  constexpr void construct(Iter begin, Iter end, size_type n) {
    if (n == 0) return;

    size_type log = log2_floor(n);
    sparse_.resize(log + 1, std::vector<value_type>(n));

    std::transform(begin, end, sparse_[0].begin(), [](const value_type &value) {
      return value;
    });

    for (size_type i = 0; i < log; ++i) {
      for (size_type j = 0; j < n; ++j) {
        size_type k = std::min(n - 1, j + (1 << i));
        sparse_[i + 1][j] = std::min(sparse_[i][j], sparse_[i][k]);
      }
    }
  }

 protected:
  sparse_type sparse_;
};

template <std::input_iterator Iter>
SparseTable(Iter, Iter, std::size_t) ->
                   SparseTable<typename std::iterator_traits<Iter>::value_type>;

} // <--- namespace yLAB

