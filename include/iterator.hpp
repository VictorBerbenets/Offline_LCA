#pragma once

#include <iterator>
#include <memory>
#include <cstddef>

#include "node.hpp"

namespace yLAB {

template <typename> class Treap;

template<typename KeyT, typename Priority>
class TreeIterator final {
  using node_type = detail::Node<KeyT, Priority>;
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type        = detail::BaseNode<node_type>;
  using pointer           = value_type*;
  using reference         = value_type&;
  using const_pointer     = const value_type*;
  using const_reference   = const value_type&;
  using difference_type   = std::ptrdiff_t;

  TreeIterator() = default;

  TreeIterator& operator++() {
    ptr_ = node_type::successor(static_cast<node_type*>(ptr_));
    return *this;
  }

  TreeIterator& operator--() {
    ptr_ = node_type::predecessor(ptr_);
    return *this;
  }

  TreeIterator operator++(int n) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  TreeIterator operator--(int n) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  const Priority& operator*() const noexcept {
    return static_cast<node_type*>(ptr_)->priority_;
  }
  const Priority* operator->() const noexcept {
    return std::addressof(static_cast<node_type*>(ptr_)->priority_);
  }

  auto operator<=>(const TreeIterator&) const = default;

  template<typename> friend class Treap;
 private:
/*----------------------------------------------------------------------------------*/
  pointer ptr_ {nullptr};

  TreeIterator(pointer ptr): ptr_ {ptr} {}
};

} // <--- namespace yLAB
