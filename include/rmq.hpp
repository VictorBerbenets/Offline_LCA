#pragma once

#include <stack>
#include <vector>
#include <iterator>
#include <utility>
#include <climits>
#include <bitset>
#include <unordered_set>

#include "cartesian_tree.hpp"
#include "sparse_table.hpp"

namespace yLAB {

template <typename T>
class RmqSolver final {
 public:
  using value_type  = T;
  using size_type   = std::size_t;
 private:
  using tree_type   = Treap<value_type>;
  using sq_table    = std::vector<std::vector<size_type>>;
  using block_bits  = std::bitset<sizeof(int) * CHAR_BIT>;
 public:

  template <std::input_iterator Iter>
  RmqSolver(Iter begin, Iter end) {
    euler_tour(begin, end);
    rmq_plus_minus_1();
  }

 private:
  template <std::input_iterator Iter>
  void euler_tour(Iter begin, Iter end) {
    tree_type tree (begin, end);
    if (!tree.root_) { return ; }

    auto vertex_num      = tree.size();
    auto euler_tour_size = 2 * vertex_num - 1;
    euler_tour_.reserve(euler_tour_size);
    heights_.reserve(euler_tour_size);
    first_appear_.reserve(vertex_num);

    std::unordered_set<typename tree_type::key_type> visited;
    std::stack<std::pair<typename tree_type::pointer, int>> stack;

    auto curr_pair = std::make_pair(tree.root_, 0);
    // for state: 0 => initial visit, 1 => just did left, 2 => just did right
    for (size_type state {0}, path {0}; curr_pair.first; ++path) {
      if (visited.find(curr_pair.first->key()) == visited.end()) {
        visited.insert(curr_pair.first->key());
        first_appear_.push_back(path);
      }
      euler_tour_.push_back(curr_pair.first->priority());
      heights_.push_back(curr_pair.second);

      if (curr_pair.first->left() && state < 1) {
        stack.push(curr_pair);
        curr_pair = std::make_pair(curr_pair.first->left(), curr_pair.second + 1);
        state = 0;
        continue;
      }
      if (curr_pair.first->right() && state < 2) {
        stack.push(curr_pair);
        curr_pair = std::make_pair(curr_pair.first->right(), curr_pair.second + 1);
        state = 0;
        continue;
      }

      if (stack.empty()) { break; }

      auto child = curr_pair.first;
      curr_pair  = stack.top();
      stack.pop();
      state = (child == curr_pair.first->left() ? 1 : 2);
    }
  }

  void rmq_plus_minus_1() {
    size_type block_sz = 1;
    if (auto log = log2_floor(heights_.size()); log > 2) {
      block_sz = log / 2; 
    }

    build_sparse_table(block_sz);
    precount_minimums_in_blocks(block_sz);
  }
 
  void build_sparse_table(size_type block_sz) {
    size_type size = heights_.size();
    size_type blocks_num = size / block_sz + (size % block_sz ? 1 : 0);

    std::vector<size_type> blocks_mins;
    blocks_mins.reserve(blocks_num);
    block_types_.reserve(blocks_num);
    size_type block_id = 0, block_num = 0, type = 0, min = heights_[0];
    for (size_type id = 0; id < size; ++id, ++block_id) {
      if (block_id < block_sz) {
        if (heights_[id] < min) {
          min = heights_[id];
        }
        if (block_id + 1 < block_sz && id + 1 < size) {
          type += heights_[id] < heights_[id + 1] ? 1 << block_id : 0;
        }
      } else if (block_id == block_sz) {
        blocks_mins.push_back(std::exchange(min, heights_[id]));
        block_types_[block_num++] = type;
        block_id = 0;
      }
    }
    if (block_id != block_sz + 1) {
      blocks_mins.push_back(min);
      block_types_[block_num] = type;
    }
    sparse_table_.construct(blocks_mins.begin(), blocks_mins.end(),
                            blocks_mins.size());
  }

  void precount_minimums_in_blocks(size_type block_sz) {
    // we have 2^(block_sz - 1)  different blocks
    size_type diff_blocks = 1 << (block_sz - 1);
    sections_mins_.assign(diff_blocks, sq_table(block_sz,
                                                std::vector<size_type>(block_sz)));
    for (size_type i = 0; i < diff_blocks; ++i) {
      auto section = get_block_section(i, block_sz);
      for (size_type j = 0; j < block_sz; ++j) {
        int min = section[j], min_id = j;
        sections_mins_[i][j][j] = j;
        for (size_type k = j + 1; k < block_sz; ++k) {
          if (min < section[k]) {
            sections_mins_[i][j][k] = min_id;
          } else {
            sections_mins_[i][j][k] = min_id = k;
            min = section[min_id];
          }
        }
      }
    }
  }

  std::vector<int> get_block_section(size_type block_id,
                                     size_type block_sz) const noexcept {
    block_bits b_set(block_id);
    std::vector<int> section(block_sz, 0);
    int assign = 0;
    for (size_type i = 1; i < block_sz; ++i) {
      if (b_set[i - 1] == 0) {
        section[i] = --assign; 
      } else {
        section[i] = ++assign; 
      }
    }
    return section;
  }

 private:
  std::vector<size_type> euler_tour_;
  std::vector<size_type> heights_;
  std::vector<size_type> first_appear_;
  std::vector<size_type> block_types_;
  std::vector<sq_table>  sections_mins_;
  SparseTable<value_type> sparse_table_;
};

} // <--- namespace yLAB

