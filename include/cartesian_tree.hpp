#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <iterator>
#include <concepts>
#include <cstddef>
#include <initializer_list>

#include "node.hpp"
#include "iterator.hpp"

namespace yLAB {

namespace dt = detail;

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
    
    std::stack<node_type*> build_nodes;
    root_ = create_node<node_type>(0, *(begin++));
    build_nodes.push(root_);
    for (size_type order_num {1}; begin != end; begin++) {
      node_type *top = nullptr;
      while (!build_nodes.empty()) {
        top = build_nodes.top();
        if (top->priority_ < *begin) {
          auto new_node = create_node<node_type>(order_num++, *begin, nullptr,
                                                 top->right_, top);
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
        build_nodes.push(create_node<node_type>(order_num++, *begin, nullptr,
                                                top, nullptr));
        top->parent_ = build_nodes.top();
        root_ = static_cast<node_type*>(top->parent_);
      }
    }
    make_root_links();
  }

  static Treap merge(const Treap &left, const Treap &right) {
	Treap result;
	result.root_ = result.merge_impl(left.root_, right.root_);
  	result.make_root_links();
	
	return result;
  }
 
  size_type size() const noexcept { return storage_.size(); }
  [[nodiscard]] bool empty() const noexcept { return storage_.size() == 0; }

  iterator begin() const noexcept { return {begin_node_}; }
  iterator end()   const noexcept { return {end_node_};   }
  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend()   const noexcept { return end();   }
  reverse_iterator rbegin() const { return std::make_reverse_iterator(end());   }
  reverse_iterator rend()   const { return std::make_reverse_iterator(begin()); }
  const_reverse_iterator crbegin() const { return std::make_reverse_iterator(cend());   }
  const_reverse_iterator crend()   const { return std::make_reverse_iterator(cbegin()); } 
 private:
  
  void make_root_links() noexcept {
    end_node_->left_ = root_;
	if (root_) {
      root_->parent_ = end_node_;
      begin_node_    = node_type::get_most_left(root_);
	}
  }

  template <typename NodeType, typename... Args>
  NodeType *create_node(Args&&... args) {
    auto un_ptr = std::make_unique<NodeType>(std::forward<Args>(args)...);
    storage_.push_back(std::move(un_ptr));
    return static_cast<NodeType*>(storage_.back().get());
  }

  // it works only if all keys in right are bigger than in left
  node_type *merge_impl(node_type *left, node_type *right) {
	if (!left)  { return right; }
	if (!right) { return left;  }

	if (left->priority_ < right->priority_) {
      auto new_right = merge_impl(left->right_, right);
	  return create_node<node_type>(left->key_, left->priority_, new_right,
                                    left->left_, left);
	}
    auto new_left = merge_impl(left, right->left_);
	return create_node<node_type>(right->key_, right->priority_, right->right_,
                                  new_left, right);
  }

  /* TODO
  * split()
  * insert()
  * erase()
  */
 private:
  std::vector<std::unique_ptr<base_node>> storage_;

  node_type *root_ {nullptr};
  base_node *end_node_;
  base_node *begin_node_ {end_node_};
};

template <std::input_iterator Iter>
Treap(Iter, Iter) -> Treap<typename std::iterator_traits<Iter>::value_type>;

} // <--- namespace yLAB

