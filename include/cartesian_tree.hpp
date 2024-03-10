#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <iterator>
#include <concepts>
#include <cstddef>

#include "node.hpp"
#include "iterator.hpp"

namespace yLAB {

template <typename T>
class Treap {
 public:
  using size_type       = std::size_t;
  using key_type        = size_type;
  using value_type      = T;
  using difference_type = std::ptrdiff_t;
  using reference       = value_type&;
  using const_reference = const value_type&;

 private:
  using node_type = detail::Node<key_type, value_type>;
 public:

  constexpr Treap() = default;
  
  // Complexity O(n)
  template <std::input_iterator Iter>
  requires requires(Iter it) { {*it} -> std::convertible_to<value_type>; }
  Treap(Iter begin, Iter end) {
    if (begin == end) return ;
    
    std::stack<node_type*> build_nodes;
    root_ = create_node(0, *(begin++));
    build_nodes.push(root_);
    for (size_type order_num {1}; begin != end; begin++) {
      node_type *top = nullptr;
      while (!build_nodes.empty()) {
        top = build_nodes.top();
        if (top->priority_ < *begin) {
          auto new_node = create_node(order_num++, *begin, nullptr, top->right_, top);
          if (top->right_) {
            top->right_->parent_ = new_node;
          }
          top->right_ = new_node;
          build_nodes.push(new_node);
          break;
        }
        build_nodes.pop();
      }
      if (build_nodes.empty()) {
        build_nodes.push(create_node(order_num++, *begin, nullptr, top, nullptr));
        root_ = top->parent_ = build_nodes.top();
      }
    }
  }
 
  size_type size() const noexcept { return storage_.size(); }
  [[nodiscard]] bool empty() const noexcept { return storage_.size() == 0; }
 
 private:

  template <typename... Args>
  node_type *create_node(Args&&... args) {
    auto un_ptr = std::make_unique<node_type>(std::forward<Args>(args)...);
    storage_.push_back(std::move(un_ptr));
    return storage_.back().get();
  }

  /* TODO
  * merge()
  * split()
  * insert()
  * erase()
  * find()
  */
 private:
  node_type *root_;
  std::vector<std::unique_ptr<node_type>> storage_;
};

} // <--- namespace yLAB

