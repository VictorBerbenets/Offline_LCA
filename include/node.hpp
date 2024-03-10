#pragma once

namespace yLAB {

template <typename> class Treap;

namespace detail {

template <typename Key, typename Priority>
class Node {
 public:
  using key_type      = Key;
  using priority_type = Priority;
  
  Node(const key_type &key, const priority_type &priority, Node *right = nullptr,
       Node *left = nullptr, Node *parent = nullptr)
      : key_ {key}, priority_ {priority},
        left_ {left}, right_ {right},
        parent_ {parent} {}

  template <typename> friend class yLAB::Treap;
 private:
  key_type key_;
  priority_type priority_;

  Node *left_, *right_, *parent_;
};

} // <--- namespace detail

} // <--- namespace yLAB

