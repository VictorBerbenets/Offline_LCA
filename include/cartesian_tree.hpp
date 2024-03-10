

namespace yLAB {

template <typename> class Treap;

namespace detail {

template <typename Key, typename Priority>
class Node {
 public:
  using key_type      = Key;
  using priority_type = Priority;

  template <typename T> friend class Treap;
 private:
  key_type key_;
  priority_type priority_;
};

template <typename T>
class Treap {

};

} // <--- namespace detail

} // <--- namespace yLAB

