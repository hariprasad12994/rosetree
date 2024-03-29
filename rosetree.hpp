#ifndef ROSETREE_H
#define ROSETREE_H

#include <algorithm>
#include <utility>
#include <queue>
#include <stack>
#include <iostream>



template <typename Tree>
class tree_as_level_order;

template <typename Tree>
class tree_as_post_order;

template <typename Tree>
class tree_as_pre_order;

template <typename T>
class Tree;

template <typename T>
typename Tree<T>::TreeNode* copy_util(typename Tree<T>::TreeNode* other);

template <typename T>
typename Tree<T>::TreeNode* copy(typename Tree<T>::TreeNode* other);


template <typename TreeClass>
struct PostOrderIterator{
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename TreeClass::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    
    using node = typename TreeClass::node;
    using node_pointer = node*;
    using node_reference = node&;

    PostOrderIterator(node_pointer ptr) {
      auto walker = ptr;
      while(walker != nullptr) {
        s.push(walker);
        walker = walker->first_child;
      }
      if(!s.empty()) {
        ptr_ = s.top(); 
      }
      else { ptr_ = nullptr; }
    }
    
    auto operator*() const -> reference { return ptr_->value; }

    auto operator->() -> node_pointer { return ptr_; }

    auto operator++() -> PostOrderIterator& {
      if(s.empty()) {
        ptr_ = nullptr;
        return *this;
      }

      auto tmp = s.top();
      s.pop();
      if(!s.empty()){
        if(tmp->first_sibling != nullptr) {
          s.push(tmp->first_sibling);
          auto walker = tmp->first_sibling->first_child;
          while(walker != nullptr) {
            s.push(walker);
            walker = walker->first_child;
          }
        }
        ptr_ = s.top();
      }
      else{ ptr_ = nullptr; }
      return *this;
    }

    auto operator++(int) -> PostOrderIterator {
      PostOrderIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const PostOrderIterator& a, const PostOrderIterator& b) { return a.ptr_ == b.ptr_; }
    friend bool operator!=(const PostOrderIterator& a, const PostOrderIterator& b) { return a.ptr_ != b.ptr_; }
    friend class Tree<value_type>;

  private:
    node_pointer ptr_;
    std::stack<node_pointer> s;
};

template <typename TreeClass>
struct PreOrderIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename TreeClass::value_type;
    using pointer = value_type*;
    using reference = value_type&;

    using node = typename TreeClass::node;
    using node_pointer = node*;
    using node_reference = node&;

    PreOrderIterator(): ptr_(nullptr) {}
    PreOrderIterator(node_pointer ptr): ptr_(ptr) {
      s.push(ptr);
    }

    auto operator*() const -> reference {
      return ptr_->value;
    }

    auto operator->() -> node_pointer {
      return ptr_;
    }

    auto operator++() -> PreOrderIterator& {
      if(s.top()->first_child != nullptr) {
          s.push(s.top()->first_child);
      }
      else if(s.top()->first_sibling != nullptr) {
          auto tmp = s.top();
          s.pop();
          s.push(tmp->first_sibling);
      }
      else{
        while(!s.empty() && s.top()->first_sibling == nullptr) {
          s.pop();
        }
        if(!s.empty()) {
            auto tmp = s.top();
            s.pop();
            s.push(tmp->first_sibling);
        }
      }
      if(!s.empty()) {
          ptr_ = s.top();
      }
      else {
          ptr_ = nullptr;
      }

      return *this;
    }

    auto operator++(int) -> PreOrderIterator {
      PreOrderIterator tmp  =*this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const PreOrderIterator& a, const PreOrderIterator& b) { return a.ptr_ == b.ptr_; }
    friend bool operator!=(const PreOrderIterator& a, const PreOrderIterator& b) { return a.ptr_ != b.ptr_; }
    friend class tree_as_level_order<TreeClass>;
    friend class tree_as_post_order<TreeClass>;
    friend class tree_as_pre_order<TreeClass>;
    friend class Tree<value_type>;

  private:
    node_pointer ptr_;
    std::stack<node_pointer> s;
};

template <typename Tree>
struct LevelOrderIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename Tree::value_type;
    using pointer = value_type*;
    using reference = value_type&;

    using node = typename Tree::node;
    using node_pointer = node*;
    using node_reference = node&;

    LevelOrderIterator(node_pointer ptr): ptr_(ptr) {}

    auto operator*() const -> reference {
      return ptr_->value;
    }

    auto operator->() -> node_pointer {
      return ptr_;
    }

    auto operator++() -> LevelOrderIterator& {
      node_pointer walker = ptr_->first_child;
      while(walker != nullptr){
        q.push(walker);
        walker = walker->first_sibling;
      }
      if(!q.empty()){
        ptr_ = q.front();
        q.pop();
      }
      else{
        ptr_ = nullptr;
      }
      return *this;
    }

    auto operator++(int) -> LevelOrderIterator {
      LevelOrderIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const LevelOrderIterator& a, const LevelOrderIterator& b) { return a.ptr_ == b.ptr_; }
    friend bool operator!=(const LevelOrderIterator& a, const LevelOrderIterator& b) { return a.ptr_ != b.ptr_; }

  private:
    std::queue<node_pointer> q;
    node_pointer ptr_;
};

template <typename T>
class Tree {
  private:
  class Node {
    public:
      T value;
      Node* first_child;
      Node* first_sibling;

      Node(): first_child(nullptr), first_sibling(nullptr) {}
      Node(const T& val): value(val), first_child(nullptr), first_sibling(nullptr) {}
      Node(T&& val): value(std::move(val)), first_child(nullptr), first_sibling(nullptr) {}
      template <typename ... Args>
      Node(Args&&... args): value(T(std::forward<Args>(args)...)), first_child(nullptr), first_sibling(nullptr) {}
  };

  public:
  using value_type = T;
  using TreeNode = typename Tree<T>::Node;
  using node = typename Tree<T>::Node;
  using node_pointer = typename Tree<T>::Node*;
  using iterator = PreOrderIterator<Tree<T>>;

  Tree() = delete;
  template <typename ... Args>
  Tree(Args ... args): tree(new typename Tree<T>::TreeNode(std::forward<Args>(args)...)) { }
  Tree(const Tree& other) {
    typename Tree<T>::TreeNode* root = copy<T>(other.tree);
    tree = root;
  }
  Tree(Tree&& other) {
    std::swap(tree, other.tree);
    other.tree = nullptr;
  }
  Tree& operator=(const Tree& other) { 
    typename Tree<T>::TreeNode* root = copy<T>(other.tree);
    tree = root;
    return *this;
  }
  Tree& operator=(Tree&& other) { 
    std::swap(tree, other.tree); 
    other.tree = nullptr; 
    return *this; 
  }
  ~Tree() { clear(); }

  private:
  typename Tree<T>::TreeNode* tree;
  Tree(typename Tree<T>::TreeNode* head): tree(head) { }
  friend typename Tree<T>::TreeNode* copy_util<T>(typename Tree<T>::TreeNode* other);
  friend typename Tree<T>::TreeNode* copy<T>(typename Tree<T>::TreeNode* other);

  public:
  auto begin() -> iterator {
    return iterator(tree);
  }

  auto end() -> iterator {
    return iterator(nullptr);
  }
  
  auto exchange_nodes(node_pointer& left, node_pointer& middle, node_pointer& right) -> iterator {
    left = std::exchange(middle, right);
    return iterator(right);
  }

  auto insert_below(iterator node, node_pointer& new_node) -> iterator {
    if(node->first_child == nullptr) {
      node->first_child = new_node;
      return iterator(new_node);
    }
    return exchange_nodes(new_node->first_sibling, node->first_child, new_node);
  }
  
  auto insert_below(iterator node, const T& data) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(data);
    return insert_below(node, new_node);
  }

  auto insert_below(iterator node, T&& data) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(std::move(data));
    return insert_below(node, new_node); 
  }

  template <typename ... Args>
  auto emplace_below(iterator node, Args ... args) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(std::forward<Args>(args)...);
    return insert_below(node, new_node);
  }

  auto insert_after(iterator node, node_pointer& new_node) -> iterator {
    if(node->first_sibling == nullptr) {
      node->first_sibling = new_node;
      return iterator(new_node);
    }
    return exchange_nodes(new_node->first_sibling, node->first_sibling, new_node);
  }

  auto insert_after(iterator node, const T& data) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(data);
    return insert_after(node, new_node);
  }

  auto insert_after(iterator node, T&& data) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(std::move(data));
    return insert_after(node, new_node);
  }

  template <typename ... Args>
  auto emplace_after(iterator node, Args ... args) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(std::forward<Args>(args)...);
    return insert_after(node, new_node);
  }

  auto append_child(iterator node, node_pointer& new_node) -> iterator {
    if(node->first_child == nullptr) {
      node->first_child = new_node;
      return iterator(new_node);
    }

    auto walker = node->first_child;
    if(walker->first_sibling == nullptr) {
      walker->first_sibling = new_node;
      return iterator(new_node);
    }
    while(walker->first_sibling != nullptr) {
      walker = walker->first_sibling;
    }
    return insert_after(walker, new_node);
  }

  auto append_child(iterator node, const T& data) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(data);
    return append_child(node, new_node);
  }

  auto append_child(iterator node, T&& data) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(std::move(data));
    return append_child(node, new_node);
  }
  
  template <typename ... Args>
  auto append_child_by_emplace(iterator node, Args ... args) -> iterator {
    typename Tree<T>::TreeNode* new_node = new typename Tree<T>::TreeNode(std::forward<Args>(args)...);
    return append_child(node, new_node);
  }

  auto empty() -> bool {
    return tree == nullptr;
  }

  auto size() -> std::size_t {
    std::size_t size = 0;
    std::for_each(begin(), end(), [&size](auto const& elem){ size++; });
    return size;
  }

  auto depth(typename Tree<T>::TreeNode* root) -> std::size_t {
    if(root == nullptr)
      return 0;
    return std::max(1 + depth(root->first_child), depth(root->first_sibling));
  }

  auto depth() -> std::size_t {
    return depth(tree);
  }

  auto cut(iterator node) -> Tree<T> {
    if(node != begin()) {
      iterator left_sibling = iterator(nullptr);
      for(auto it = begin(); it != end(); it++) {
        if(it->first_sibling == node) {
          left_sibling = it;
          break;
        }
      }
      left_sibling->first_sibling = node->first_sibling;
      node->first_sibling = nullptr;
    }
    else {
      if(node->first_sibling != nullptr) { tree = node->first_sibling; }
      else { tree = nullptr; }
    }
    return Tree(node.ptr_);
  }

  auto erase_intern(iterator node) -> void {
    auto temp = tree_as_post_order(node);
    for(auto it = temp.begin(); it != temp.end(); it++) {
      delete(it.ptr_);
    }
  }

  auto erase(iterator node) -> void {
    if(node == end()) { return; }
    auto tree_to_be_erased = cut(node);
  }

  auto clear() -> void {
    erase_intern(begin());
    tree = nullptr;
  }
};

template <typename Tree>
class tree_as_level_order {
  private:
    using iterator = PreOrderIterator<Tree>;
    using level_iterator = LevelOrderIterator<Tree>;
    level_iterator begin_;
    level_iterator end_;

  public:
    tree_as_level_order(const iterator& it): begin_(it.ptr_), end_(nullptr) {}
    level_iterator begin() { return begin_; }
    level_iterator end() { return end_; }
};

template <typename Tree>
class tree_as_post_order {
  private:
    using iterator = PreOrderIterator<Tree>;
    using post_order_iterator = PostOrderIterator<Tree>;
    post_order_iterator begin_;
    post_order_iterator end_;

  public:
    tree_as_post_order(const iterator& it): begin_(it.ptr_), end_(nullptr) {}
    post_order_iterator begin() { return begin_; }
    post_order_iterator end() { return end_; }
};

template<typename Tree>
class tree_as_pre_order{
  private:
    using iterator = PreOrderIterator<Tree>;
    using pre_order_iterator = PreOrderIterator<Tree>;
    pre_order_iterator begin_;
    pre_order_iterator end_;

  public:
    tree_as_pre_order(const iterator& it) {
      begin_ = it.ptr_;
      if(it.ptr_ == nullptr) { end_ = nullptr; }
      else { end_ = it.ptr_->first_sibling; }
    }

    pre_order_iterator begin() { return begin_; }
    pre_order_iterator end() { return end_; }
};

template <typename T, typename IteratorProxy>
auto tree_to_sstream(PreOrderIterator<Tree<T>> tree, std::stringstream& stream) -> void {
  for(auto tree_elem: IteratorProxy(tree)) {
    stream << tree_elem << " ";
  }
}

template <typename T>
typename Tree<T>::TreeNode* copy_util(typename Tree<T>::TreeNode* ptr) {
  if(ptr == nullptr) 
    return nullptr;

  auto curr_node = new typename Tree<T>::TreeNode(ptr->value);
  curr_node->first_child = copy_util<T>(ptr->first_child);
  curr_node->first_sibling = copy_util<T>(ptr->first_sibling);
  return curr_node;
}

template <typename T>
typename Tree<T>::TreeNode* copy(typename Tree<T>::TreeNode* other) {
  return copy_util<T>(other);
}

#endif /*ROSETREE_H*/

