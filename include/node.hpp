#pragma once

namespace yLAB {

template <typename> class Treap;

namespace detail {

template <typename Key, typename Priority>
class Node final {
 public:
  using key_type      = Key;
  using priority_type = Priority;
  using pointer       = Node*;
  using const_pointer = const Node*;
  Node(const key_type &key, const priority_type &priority, Node *right = nullptr,
       Node *left = nullptr, Node *parent = nullptr)
      : key_ {key}, priority_ {priority},
        left_ {left}, right_ {right},
        parent_ {parent} {}

  Node *successor() {
    if (right_) {
      return get_most_left(right_);
    }
    return const_cast<pointer>(go_upper_inc());
  }

  Node *predecessor() {
    if (left_) {
      return get_most_right(left_);
    }
    return const_cast<pointer>(go_upper_dec());
  } 

  template <typename> friend class yLAB::Treap;
 private:
  static auto get_most_right(pointer start) noexcept {
    while(start->right_) {
      start = start->right_;
    }
    return start;
  }

  static auto get_most_left(pointer start) noexcept {
    while(start->left_) {
      start = start->left_;
    }
    return start;
  }

  auto go_upper_dec() const {
    auto tmp = parent_;
    auto copy = this;
    while (copy == tmp->left_) {
      copy = std::exchange(tmp, tmp->parent_);
    }
    if (copy->left_ != tmp) {
      copy = tmp;
    }
    return copy;
  }

  auto go_upper_inc() const {
    auto tmp = parent_;
    auto copy = this;
    while (copy == tmp->right_) {
      copy = std::exchange(tmp, tmp->parent_);
    }
    if (copy->right_ != tmp) {
      copy = tmp;
    }
    return copy;
  }
 private:
  key_type key_;
  priority_type priority_;

  Node *left_, *right_, *parent_;
};

} // <--- namespace detail

} // <--- namespace yLAB
