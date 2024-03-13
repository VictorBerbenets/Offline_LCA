#pragma once

#include <stack>
#include <vector>
#include <iterator>
#include <utility>
#include <unordered_set>

#include "cartesian_tree.hpp"

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
  RmqSolver(Iter begin, Iter end) 
 	  : tree_ (begin, end) {
    euler_tour(); 
  }

 private:
  void euler_tour() {
	  if (!tree_.root_) { return ; }

    auto vertex_num      = tree_.size();
    auto euler_tour_size = 2 * vertex_num - 1;
    euler_tour_.reserve(euler_tour_size);
    heights_.reserve(euler_tour_size);
    first_appear_.reserve(vertex_num);

    std::unordered_set<typename tree_type::key_type> visited;
    std::stack<std::pair<typename tree_type::pointer, int>> stack;

    auto curr_pair = std::make_pair(tree_.root_, 0);
    // for state: 0 => initial visit, 1 => just did left, 2 => just did right
    for (size_type state {0}, path {0}; curr_pair.first; ++path) {
      if (visited.find(curr_pair.first->key_) == visited.end()) {
        visited.insert(curr_pair.first->key_);
        first_appear_.push_back(path);
      }
      euler_tour_.push_back(curr_pair.first->priority_);
      heights_.push_back(curr_pair.second);

      if (curr_pair.first->left_ && state < 1) {
        stack.push(curr_pair);
        curr_pair = std::make_pair(curr_pair.first->left_, curr_pair.second + 1);
        state = 0;
        continue;
      }
      if (curr_pair.first->right_ && state < 2) {
        stack.push(curr_pair);
        curr_pair = std::make_pair(curr_pair.first->right_, curr_pair.second + 1);
        state = 0;
        continue;
      }

      if (stack.empty()) { break; }

      auto child = curr_pair.first;
      curr_pair  = stack.top();
      stack.pop();
      state = (child == curr_pair.first->left_ ? 1 : 2);
    }
  }
	
 private:
  std::vector<size_type> euler_tour_;
  std::vector<size_type> heights_;
  std::vector<size_type> first_appear_;
  tree_type tree_;
};

} // <--- namespace yLAB

