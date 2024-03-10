

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
      : key_ {key}, priority_ {priority_},
        left_ {left}, right_ {right},
        parent_ {parent} {}

  template <typename T> friend class Treap;
 private:
  key_type key_;
  priority_type priority_;

  Node *left_, *right_, *parent_;
};

template <typename T>
class Treap {

};

} // <--- namespace detail

} // <--- namespace yLAB

