#pragma once

#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <cmath>
#include <bit>

namespace yLAB {

template <typename T>
class SparseTable final {
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

  constexpr value_type min(const std::pair<size_type, size_type> &query) const {
    if (query.first > query.second) {
      throw std::runtime_error {"invalid query range"};
    }

    int i = log2_floor(query.second - query.first + 1);
    return std::min(sparse_[i][query.first],
                    sparse_[i][query.second - (1 << i) + 1]);
  }

 private:
  template <std::input_iterator Iter>
  constexpr void construct(Iter begin, Iter end, size_type n) {
    if (n == 0) return;

    sparse_.resize(log2_floor(n) + 1, std::vector<value_type>(n));

    size_type log = log2_floor(n);
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

  constexpr int log2_floor(size_type number) const noexcept {
    return std::bit_width(number) - 1;
  }
 private:
  sparse_type sparse_;
};

template <std::input_iterator Iter>
SparseTable(Iter, Iter, std::size_t) ->
                   SparseTable<typename std::iterator_traits<Iter>::value_type>;

} // <--- namespace yLAB

