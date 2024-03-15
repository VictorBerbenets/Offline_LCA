#pragma once

#include <stack>
#include <vector>
#include <iterator>
#include <utility>
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
 public:

  template <std::input_iterator Iter>
  RmqSolver(Iter begin, Iter end) {
    euler_tour(begin, end); 
  }

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
    size_type size = heights_.size();
    size_type block_sz = 1;
    if (auto log = log2_floor(size); log > 2) {
      block_sz = log / 2; 
    }
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
        if (block_id + 1 < block_sz) {
          type += heights_[id] < heights_[id + 1] ? 0 : 1 << block_id;
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

 private:
  std::vector<size_type> euler_tour_;
  std::vector<size_type> heights_;
  std::vector<size_type> first_appear_;
  std::vector<size_type> block_types_;
  SparseTable<value_type> sparse_table_;
};

} // <--- namespace yLAB

