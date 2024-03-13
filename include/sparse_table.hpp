#pragma once

#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>
#include <cmath>
#include <bit>

namespace yLAB {

template <typename T>
class SparseTable final {
 public:
  using size_type   = std::size_t;
  using value_type  = T;
  using sparse_type = std::vector<std::vector<value_type>>;

  template <std::forward_iterator Iter>
  constexpr SparseTable(Iter begin, Iter end, size_type n)
      : sparse_ (log2_floor(n) + 1, std::vector<value_type>(n)) {
    size_type log = log2_floor(n);
    std::transform(begin, end, sparse_[0].begin(), [](const value_type &value) {
      return value;
    });

    for (size_type i = 0; i < log; ++i) {
      for (size_type j = 0; j < n; ++j) {
        size_type k = std::min(n, j + (1 << i));
        sparse_[i + 1][j] = std::min(sparse_[i][j], sparse_[i][k]);
      }
    }
  }

  value_type min(const std::pair<size_type, size_type> &query) const {
    if (query.first > query.second) {
      throw std::runtime_error {"invalid query range"};
    }

    int i = log2_floor(query.second - query.first + 1);
    return std::min(sparse_[i][query.first],
                    sparse_[i][query.second - (1 << i) + 1]);
  }

 private:
  constexpr int log2_floor(size_type number) const noexcept {
    return std::bit_width(number) - 1; 
  }
 private:
  sparse_type sparse_;
};

} // <--- namespace yLAB

