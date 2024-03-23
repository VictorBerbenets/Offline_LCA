#pragma once

#include <stack>
#include <vector>
#include <iterator>
#include <utility>
#include <climits>
#include <bitset>
#include <initializer_list>

#include "cartesian_tree.hpp"
#include "sparse_table.hpp"

namespace yLAB {

template <typename T>
class RmqSolver final: private SparseTable<std::size_t> {
 public:
  using value_type  = T;
  using size_type   = std::size_t;
 private:
  using tree_type   = Treap<value_type>;
  using sq_table    = std::vector<std::vector<size_type>>;
  using block_bits  = std::bitset<sizeof(int) * CHAR_BIT>;

  using sparse_table = SparseTable<size_type>;
  using sparse_table::sparse_;
 public:

  RmqSolver(std::initializer_list<value_type> i_list)
      : RmqSolver(i_list.begin(), i_list.end()) {}

  template <std::input_iterator Iter>
  RmqSolver(Iter begin, Iter end) {
    euler_tour(begin, end);
    rmq_plus_minus_1();
  }

  value_type ans_query(const std::pair<size_type, size_type> &query) const {
    auto [left_id, right_id] = get_heights_positions(query);
    if (left_id > right_id) {
      std::swap(left_id, right_id);
    }
    return euler_tour_[rmq({left_id, right_id})];
  }

 private:
  size_type rmq(const std::pair<size_type, size_type> &query) const {
   auto left_block  = query.first / block_sz_;
   auto right_block = query.second / block_sz_;
   // if both indexes are inside the same block
   if (left_block == right_block) {
     return block_rmq(left_block, query.first % block_sz_, query.second % block_sz_);
   }
   // find the minimum on the segment from l to the end of the block containing l
   auto ansl = block_rmq(left_block, query.first % block_sz_, block_sz_ - 1);
   // find the minimum from the beginning of the block containing r to r
   auto ansr = block_rmq(right_block, 0, query.second % block_sz_);
   // find the minimum on the blocks between the outer ones, if there are any
   if (left_block + 1 < right_block) {
     auto power = log2_floor(right_block - left_block - 1);
     auto ansb  = min(sparse_[left_block + 1][power],
                      sparse_[right_block - (1 << power)][power]);
     return min(ansb, min(ansl, ansr));
   }
   return min(ansl, ansr);
  }

  template <std::input_iterator Iter>
  void euler_tour(Iter begin, Iter end) {
    tree_type tree (begin, end);
    if (!tree.root_) { return ; }

    auto vertex_num      = tree.size();
    auto euler_tour_size = 2 * vertex_num - 1;
    euler_tour_.reserve(euler_tour_size);
    heights_.reserve(euler_tour_size);
    first_appear_.assign(vertex_num, -1);

    std::stack<std::pair<typename tree_type::pointer, int>> stack;

    auto curr_pair = std::make_pair(tree.root_, 0);
    // for state: 0 => initial visit, 1 => just did left, 2 => just did right
    for (size_type state {0}, path {0}; curr_pair.first; ++path) {
      if (auto key = curr_pair.first->key(); first_appear_[key] == -1) {
        first_appear_[key] = path;
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
    if (auto log = log2_floor(heights_.size()); log > 2) {
      block_sz_ = log / 2;
    }

    auto min_blocks_pos = get_min_pos_in_each_block();
    build_sparse_table(min_blocks_pos.begin(), min_blocks_pos.end(),
                       min_blocks_pos.size());
    precompute_all_blocks_rmq();
    compute_each_block_type();
  }

  std::vector<int> get_min_pos_in_each_block() {
    size_type size = heights_.size();
    size_type blocks_num = size / block_sz_ + (size % block_sz_ ? 1 : 0);
    block_types_.assign(blocks_num, 0);
    std::vector<int> blocks_mins(blocks_num, -1);
    int curr_block = -1;
    for (size_type id = 0; id < size; ++id) {
      if (id % block_sz_ == 0) {
        ++curr_block;
      }
      if (blocks_mins[curr_block] == -1 ||
          heights_[blocks_mins[curr_block]] > heights_[id]) {
        blocks_mins[curr_block] = id;
      }
    }
    return blocks_mins;
  }

  void compute_each_block_type() {
    for (size_type i = 0, j = 0, curr_block = 0, size = heights_.size();
         i < size || j < block_sz_; ++i, ++j) {
      if (j >= block_sz_) {
        j = 0, ++curr_block;
      }
      if (j > 0 && (i >= size || heights_[i - 1] < heights_[i])) {
        block_types_[curr_block] += (1 << (j - 1));
      }
    }
  }

  void precompute_all_blocks_rmq() {
    // we have 2^(block_sz - 1)  different blocks
    size_type diff_blocks = 1 << (block_sz_ - 1);
    sections_mins_.assign(diff_blocks, sq_table(block_sz_,
                                                std::vector<size_type>(block_sz_)));
    for (size_type i = 0; i < diff_blocks; ++i) {
      auto section = get_block_section(i);
      for (size_type j = 0; j < block_sz_; ++j) {
        int min = section[j], min_id = j;
        sections_mins_[i][j][j] = j;
        for (size_type k = j + 1; k < block_sz_; ++k) {
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

  template <std::input_iterator Iter>
  void build_sparse_table(Iter begin, Iter end, size_type size) {
    size_type log = log2_floor(size);
    sparse_.resize(size, std::vector<size_type>(log + 1));

    for (int i = 0; begin != end; ++begin, ++i) {
      sparse_[i][0] = *begin;
    }
    for (size_type j = 1; j <= log; ++j) {
      for (size_type i = 0; i < size; ++i) {
        size_type ind = (1 << (j - 1)) + i;
        if (ind >= size) {
          sparse_[i][j] = sparse_[i][j - 1];
        }
        else if (heights_[sparse_[i][j - 1]] > heights_[sparse_[ind][j - 1]]) {
          sparse_[i][j] = sparse_[ind][j - 1];
        }
        else {
          sparse_[i][j] = sparse_[i][j - 1];
        }
      }
    }
  }

  std::vector<int> get_block_section(size_type block_id) const {
    block_bits b_set(block_id);
    std::vector section(block_sz_, 0);
    int assign = 0;
    for (size_type i = 1; i < block_sz_; ++i) {
      if (b_set[i - 1] == 0) {
        section[i] = --assign;
      } else {
        section[i] = ++assign;
      }
    }
    return section;
  }

  size_type min(size_type l, size_type r) const {
    return heights_[l] < heights_[r] ? l : r;
  }

  size_type block_rmq(size_type block_num, size_type l, size_type r) const {
    return sections_mins_[block_types_[block_num]][l][r] + block_num * block_sz_;
  }

  std::pair<size_type, size_type>
  get_heights_positions(const std::pair<size_type, size_type> &query) const {
    return std::make_pair(first_appear_[query.first],
                          first_appear_[query.second]);
  }

 private:
  std::vector<value_type> euler_tour_;
  std::vector<int> first_appear_;
  std::vector<size_type> heights_;
  std::vector<size_type> block_types_;
  std::vector<sq_table>  sections_mins_;
  size_type block_sz_ {1};
};

template <std::input_iterator Iter>
RmqSolver(Iter, Iter) -> RmqSolver<typename std::iterator_traits<Iter>::value_type>;

} // <--- namespace yLAB

