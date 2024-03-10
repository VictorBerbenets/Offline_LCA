#include <memory>
#include <vector>
#include <iterator>
#include <cstddef>

namespace yLAB {


namespace detail {

template <typename> class Treap;

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

  template <typename> friend class Treap;
 private:
  key_type key_;
  priority_type priority_;

  Node *left_, *right_, *parent_;
};

} // <--- namespace detail

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
  template <std::input_iterator Iter>
  Treap(Iter begin, Iter end) {

  }

 private:
  node_type *root_;
  std::vector<std::unique_ptr<node_type>> storage_;
};

} // <--- namespace yLAB

