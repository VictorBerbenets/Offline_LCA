#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <queue>
#include <iterator>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <utility>

#include "node.hpp"
#include "iterator.hpp"

namespace yLAB {

namespace dt = detail;

template <typename> class RmqSolver;

/*
 * This Treap class contains an incomplete interface (or rather,
 * it is completely absent). Its main purpose is to simply build
 * a Cartesian tree and be able to traverse it in the RmqSolver class.
*/

template <typename T>
class Treap final {
 public:
  using size_type              = std::size_t;
  using key_type               = size_type;
  using value_type             = T;
  using node_type              = dt::Node<key_type, value_type>;
  using difference_type        = std::ptrdiff_t;
  using reference              = value_type&;
  using const_reference        = const value_type&;
  using pointer                = node_type*;
  using const_pointer          = const node_type*;
  using const_iterator         = TreeIterator<key_type, value_type>;
  using iterator               = const_iterator;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 private:
  using base_node = node_type::base_node;

  using node_pointer      = typename base_node::node_pointer;
  using base_node_pointer = typename base_node::base_node_pointer;
 public:

  constexpr Treap()
      : end_node_ {create_node<base_node>(root_)} {}

  Treap(std::initializer_list<value_type> i_list)
      : Treap(i_list.begin(), i_list.end()) {}

  // Complexity O(n)
  template <std::input_iterator Iter>
  requires requires(Iter it) { {*it} -> std::convertible_to<value_type>; }
  Treap(Iter begin, Iter end)
      : end_node_ {create_node<base_node>(root_)} {
    if (begin == end) return ;

    std::stack<node_pointer> build_nodes;
    root_ = create_node<node_type>(0, *(begin++));
    build_nodes.push(root_);
    for (size_type order_num {1}; begin != end; ++begin) {
      node_pointer top = nullptr;
      while (!build_nodes.empty()) {
        top = build_nodes.top();
        if (top->priority() < *begin) {
          auto new_node = create_node<node_type>(order_num++, *begin, nullptr,
                                                 top->right(), top);
          if (top->right()) {
            top->right()->parent() = new_node;
          }
          top->right() = new_node;
          build_nodes.push(new_node);
          break;
        }
        build_nodes.pop();
      }
      if (build_nodes.empty()) {
        build_nodes.push(create_node<node_type>(order_num++, *begin, nullptr,
                                                top, nullptr));
        top->parent() = build_nodes.top();
        root_ = static_cast<node_pointer>(top->parent());
      }
    }
    make_root_links();
  }

  Treap(const Treap &rhs)
      : end_node_ {create_node<base_node>(root_)} {
    if (!rhs.size()) { return ; }
 
    root_ = create_node<node_type>(rhs.root_->key(), rhs.root_->priority());

    // level tree traversal nodes
    std::queue<node_pointer> rhs_nodes;
    // creating nodes
    std::queue<node_pointer> nodes;

    rhs_nodes.push(rhs.root_);
    nodes.push(root_);
    while(rhs_nodes.size()) {
      auto parent   = nodes.front();
      auto rhs_node = rhs_nodes.front();
      if (rhs_node->left()) {
        parent->left() = create_node<node_type>(rhs_node->left()->key(),
                                                rhs_node->left()->priority());
        parent->left()->parent() = parent;
        rhs_nodes.push(rhs_node->left());
        nodes.push(parent->left());
      }
      if (rhs_node->right()) {
        parent->right() = create_node<node_type>(rhs_node->right()->key(),
                                                 rhs_node->right()->priority());
        parent->right()->parent() = parent;
        rhs_nodes.push(rhs_node->right());
        nodes.push(parent->right());
      }
      rhs_nodes.pop();
      nodes.pop();
    }
    make_root_links();
  }
  
  Treap(Treap &&rhs) = default;
  
  Treap &operator=(const Treap &rhs) {
    if (this == std::addressof(rhs)) {
      return *this;
    }
    auto copy = rhs;
    swap(copy);
    return *this;
  }

  Treap &operator=(Treap &&rhs) noexcept {
    swap(rhs);

    return *this;
  }

  ~Treap() = default;

  // it works only if all keys in right are bigger than in left
  static Treap merge(const Treap &left, const Treap &right) {
    Treap result;
    result.root_ = result.merge_impl(left.root_, right.root_);
    result.make_root_links();

    return result;
  }

  void swap(Treap &rhs) noexcept {
    std::swap(root_, rhs.root_);
    std::swap(begin_node_, rhs.begin_node_);
    std::swap(end_node_, rhs.end_node_);
    storage_.swap(rhs.storage_);
  }

  size_type size() const noexcept { return storage_.size() - 1; }
  [[nodiscard]] bool empty() const noexcept { return storage_.size() - 1 == 0; }

  iterator begin() const noexcept { return {begin_node_}; }
  iterator end()   const noexcept { return {end_node_};   }
  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend()   const noexcept { return end();   }
  reverse_iterator rbegin() const { return std::make_reverse_iterator(end());   }
  reverse_iterator rend()   const { return std::make_reverse_iterator(begin()); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend()   const { return rbegin(); }
 private:

  void make_root_links() noexcept {
    end_node_->left() = root_;
	  if (root_) {
      root_->parent() = end_node_;
      begin_node_     = node_type::get_most_left(root_);
	  }
  }

  template <typename NodeType, typename... Args>
  NodeType *create_node(Args&&... args) {
    auto un_ptr = std::make_unique<NodeType>(std::forward<Args>(args)...);
    storage_.push_back(std::move(un_ptr));
    return static_cast<NodeType*>(storage_.back().get());
  }

  node_pointer merge_impl(node_pointer left, node_pointer right) {
    if (!left)  { return right; }
    if (!right) { return left;  }

    if (left->priority() < right->priority()) {
      auto new_right = merge_impl(left->right(), right);
      return create_node<node_type>(left->key(), left->priority(), new_right,
                                      left->left(), left);
    }
    auto new_left = merge_impl(left, right->left());
    return create_node<node_type>(right->key(), right->priority(), right->right(),
                                  new_left, right);
  }

  std::pair<node_pointer, node_pointer> split(node_pointer node, size_type key) {
    if (!node) { return {nullptr, nullptr}; }

    if (node->key() <= key) {
      auto [left, right] = split(node->right(), key);
      node->right() = left;
      return {node, right};
    } else {
      auto [left, right] = split(node->left(), key);
      node->left() = right;
      return {left, node};
    }
  }

  template <typename> friend class RmqSolver;
 private:
  std::vector<std::unique_ptr<base_node>> storage_;

  node_pointer root_ {nullptr};
  base_node_pointer end_node_;
  base_node_pointer begin_node_ {end_node_};
};

template <std::input_iterator Iter>
Treap(Iter, Iter) -> Treap<typename std::iterator_traits<Iter>::value_type>;

} // <--- namespace yLAB

