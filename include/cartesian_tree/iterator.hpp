#pragma once

#include <iterator>
#include <memory>
#include <cstddef>

#include "node.hpp"

namespace yLAB {

template <typename> class Treap;

template<typename KeyT, typename Priority>
class TreeIterator final {
  using node_type      = detail::Node<KeyT, Priority>;
  using base_node      = typename node_type::base_node;
  using key_type       = typename node_type::key_type;
  using priority_type  = typename node_type::priority_type;

  struct ProxyPair;
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type        = std::pair<key_type, priority_type>;
  using reference         = std::pair<key_type&, priority_type&>;
  using pointer           = base_node*;
  using const_pointer     = const base_node*;
  using const_reference   = std::pair<const key_type&, const priority_type&>;
  using difference_type   = std::ptrdiff_t;

  constexpr TreeIterator() = default;

  constexpr TreeIterator& operator++() {
    ptr_ = node_type::successor(static_cast<node_type*>(ptr_));
    return *this;
  }

  constexpr TreeIterator& operator--() {
    ptr_ = node_type::predecessor(ptr_);
    return *this;
  }

  constexpr TreeIterator operator++(int n) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr TreeIterator operator--(int n) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  constexpr const_reference operator*() const noexcept {
    auto node_ptr = static_cast<node_type*>(ptr_);
    return {node_ptr->key(), node_ptr->priority()};
  }

  constexpr ProxyPair operator->() const noexcept {
    return {this->operator*()};
  }

  constexpr auto operator<=>(const TreeIterator&) const = default;

  template<typename> friend class Treap;
 private:
/*----------------------------------------------------------------------------------*/
  pointer ptr_ {nullptr};

  constexpr TreeIterator(pointer ptr): ptr_ {ptr} {}
  
  struct ProxyPair final {
    const_reference *operator->() {
      return std::addressof(ref_);
    }

    const_reference ref_;
  };

};

} // <--- namespace yLAB
