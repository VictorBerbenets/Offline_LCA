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

  template <typename, typename> friend class Node;
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

  static Node *successor(node_type *node) {
    if (node->right_) {
      return get_most_left(node->right_);
    }
    return const_cast<pointer>(node->go_upper_inc());
  }

  static Node *predecessor(base_node *b_node) {
    if (b_node->left_) {
      return get_most_right(b_node->left_);
    }
    return const_cast<pointer>(static_cast<node_type*>(b_node)->go_upper_dec());
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
    auto tmp  = parent_;
    auto copy = this;
    while (copy == tmp->left_) {
      copy = static_cast<node_type*>(std::exchange(tmp,
                                       static_cast<node_type*>(tmp)->parent_));
    }
    if (copy->left_ != tmp) {
      copy = static_cast<node_type*>(tmp);
    }
    return copy;
  }

  auto go_upper_inc() const {
    auto tmp = parent_;
    auto copy = this;
    while (tmp->left_ != copy &&
           copy == static_cast<node_type*>(tmp)->right_) {
      copy = static_cast<node_type*>(std::exchange(tmp,
                                       static_cast<node_type*>(tmp)->parent_));
    }
    if (copy->right_ != tmp) {
      copy = static_cast<node_type*>(tmp);
    }
    return copy;
  }
 private:
  key_type key_;
  priority_type priority_;
  
  base_node *parent_;
  node_type *right_;
};

} // <--- namespace detail

} // <--- namespace yLAB

