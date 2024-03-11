#pragma once

namespace yLAB {

template <typename, typename> class TreeIterator;
template <typename> class Treap;

namespace detail {

template <typename, typename> class Node;

template <typename Key, typename Priority>
class BaseNode {
  using child_type = Node<Key, Priority>;
 public:
  BaseNode(child_type *child = nullptr): left_ {child} {}
  
  virtual ~BaseNode() {}

 protected:
   child_type *left_; 
};

template <typename Key, typename Priority>
class Node final: public BaseNode<Key, Priority> {
 public:
  using key_type      = Key;
  using priority_type = Priority;
  using pointer       = Node*;
  using const_pointer = const Node*;
  using base_node     = BaseNode<key_type, priority_type>;
  using node_type     = Node<key_type, priority_type>;

  Node(const key_type &key, const priority_type &priority, Node *right = nullptr,
       Node *left = nullptr, Node *parent = nullptr)
      : BaseNode<Key, Priority>(left), key_ {key},
        priority_ {priority}, parent_ {parent},
        right_ {right} {}

  Node *successor() {
    if (right_) {
      return get_most_left(static_cast<node_type*>(right_));
    }
    return const_cast<pointer>(go_upper_inc());
  }

  Node *predecessor() {
    if (this->left_) {
      return get_most_right(static_cast<node_type*>(this->left_));
    }
    return const_cast<pointer>(go_upper_dec());
  } 

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

  template <typename, typename> friend class yLAB::TreeIterator;
  template <typename> friend class yLAB::Treap;
 private:
  auto go_upper_dec() const {
    auto tmp = static_cast<node_type*>(parent_);
    auto copy = this;
    while (copy == tmp->left_) {
      copy = std::exchange(tmp, static_cast<node_type*>(tmp->parent_));
    }
    if (copy->left_ != tmp) {
      copy = tmp;
    }
    return copy;
  }

  auto go_upper_inc() const {
    auto tmp = static_cast<node_type*>(parent_);
    auto copy = this;
    while (copy == tmp->right_) {
      copy = std::exchange(tmp, static_cast<node_type*>(tmp->parent_));
    }
    if (copy->right_ != tmp) {
      copy = tmp;
    }
    return copy;
  }
 private:
  key_type key_;
  priority_type priority_;
  
  base_node *parent_;
  Node *right_;
};

} // <--- namespace detail

} // <--- namespace yLAB

