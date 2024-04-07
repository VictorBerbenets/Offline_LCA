#pragma once

#include <utility>

namespace yLAB {

namespace detail {

template <typename NodeType>
class BaseNode {
 public:
  using node_pointer      = NodeType*;
  using base_node_pointer = BaseNode*;

  constexpr BaseNode(node_pointer child = nullptr): left_ {child} {}

  virtual ~BaseNode() = default;

  constexpr auto &left() noexcept { return left_; }

 protected:
  node_pointer left_;
};

template <typename Key, typename Priority>
class Node final: public BaseNode<Node<Key, Priority>> {
 public:
  using key_type      = Key;
  using priority_type = Priority;
  using node_type     = Node<key_type, priority_type>;
  using base_node     = BaseNode<node_type>;

  using typename base_node::node_pointer;
  using typename base_node::base_node_pointer;

  constexpr Node(const key_type &key, const priority_type &priority,
       node_pointer right = nullptr, node_pointer left = nullptr,
       node_pointer parent = nullptr)
      : base_node(left), key_ {key},
        priority_ {priority}, parent_ {parent},
        right_ {right} {}
  
  ~Node() = default;

  static constexpr base_node_pointer successor(node_pointer node) {
    if (node->right_) {
      return get_most_left(node->right_);
    }
    return const_cast<node_pointer>(node->go_upper_inc());
  }

  static constexpr node_pointer predecessor(base_node_pointer b_node) {
    if (b_node->left()) {
      return get_most_right(b_node->left());
    }
    return const_cast<node_pointer>(static_cast<node_pointer>(b_node)->go_upper_dec());
  }

  static constexpr auto get_most_right(node_pointer start) noexcept {
    while(start->right_) {
      start = start->right_;
    }
    return start;
  }

  static constexpr auto get_most_left(node_pointer start) noexcept {
    while(start->left_) {
      start = start->left_;
    }
    return start;
  }

  constexpr auto &right() noexcept { return right_; }
  constexpr auto &parent() noexcept { return parent_; }
  constexpr auto &key() const noexcept { return key_; }
  constexpr auto &priority() const noexcept { return priority_; }

 private:
  constexpr auto go_upper_dec() const {
    auto parent  = parent_;
    auto self = this;
    while (self == parent->left()) {
      self = static_cast<node_pointer>(std::exchange(parent,
                                       static_cast<node_pointer>(parent)->parent_));
    }
    if (self->left_ != parent) {
      self = static_cast<node_pointer>(parent);
    }
    return self;
  }

  constexpr auto go_upper_inc() const {
    auto parent = parent_;
    auto self = this;
    while (parent->left() != self &&
           self == static_cast<node_pointer>(parent)->right_) {
      self = static_cast<node_pointer>(std::exchange(parent,
                                       static_cast<node_pointer>(parent)->parent_));
    }
    if (self->right_ != parent) {
      self = static_cast<node_pointer>(parent);
    }
    return self;
  }
 private:
  key_type key_;
  priority_type priority_;

  base_node_pointer parent_;
  node_pointer right_;
};

} // <--- namespace detail

} // <--- namespace yLAB

