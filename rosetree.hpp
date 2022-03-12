#ifndef ROSETREE_H
#define ROSETREE_H

#include <algorithm>
#include <queue>
#include <stack>
#include <iostream>

// refer - https://course.ccs.neu.edu/cs5010f18/lecture9.html
// refer - multiple iter - https://stackoverflow.com/questions/25105730/range-based-for-loops-and-multiple-iterators
// https://www.fluentcpp.com/2021/09/02/how-to-make-your-classes-compatible-with-range-for-loop/
// https://algotree.org/algorithms/tree_graph_traversal/

template <typename T>
class TreeNode {
  public:
    T value;
    TreeNode* first_child;
    TreeNode* first_sibling;

    TreeNode(): first_child(nullptr), first_sibling(nullptr) {}
    TreeNode(const T& val): value(val), first_child(nullptr), first_sibling(nullptr) {}
    TreeNode(T&& val): value(val), first_child(nullptr), first_sibling(nullptr) {}
};

template <typename Tree>
class tree_as_level_order;

template <typename Tree>
struct PostOrderIterator{
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename Tree::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    
    using node = typename Tree::node;
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
        //s.pop();
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

  private:
    node_pointer ptr_;
    std::stack<node_pointer> s;
};

template <typename Tree>
struct PreOrderIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename Tree::value_type;
    using pointer = value_type*;
    using reference = value_type&;

    using node = typename Tree::node;
    using node_pointer = node*;
    using node_reference = node&;

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
    friend class tree_as_level_order<Tree>;

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
  public:
  using value_type = T;
  using node = TreeNode<T>;
  using node_pointer = TreeNode<T>*;
  using iterator = PreOrderIterator<Tree<T>>;

  // todo? class Compare as template parameter
  // todo? class Allocator<T> as template paramter
  // todo: add special memeber functions

  Tree() {}
  Tree(TreeNode<T>* head): tree(head) {}

  auto begin() -> iterator {
    return iterator(tree);
  }

  auto end() -> iterator {
    return iterator(nullptr);
  }
  
  // refer - // https://stackoverflow.com/questions/26198350/c-stacks-push-vs-emplace/26198609
  // refer - // https://stackoverflow.com/questions/17172080/insert-vs-emplace-vs-operator-in-c-map
  //https://codereview.stackexchange.com/questions/214882/binary-search-tree-implementation-with-unique-pointers
  // todo: switch to unique_ptr for leak protection
  // todo: dfs iterators - pre, post and inorder traversal, sibling iterator
  // todo: insert at, insert after, insert below
  // todo: emplace at, emplace after, emplace below, try emplace
  // todo: append_child, prepend_child
  // todo: clear, erase, delete at
  // todo: merge
  // todo: paths, relationship APIs
  // todo: [] operator
  // todo: shallow and depp copy friendly iterators
  // todo: traversal caches
  //
  auto insert_below(iterator node, T data) -> void {
    TreeNode<T>* new_node = new TreeNode<T>(data);
    if(node->first_child == nullptr){
      node->first_child = new_node;
      return;
    }
    auto tmp = node->first_child;
    node->first_child = new_node;
    new_node->first_sibling = tmp;
  }

  auto insert_below(T key, T data) -> void {
    iterator it = std::find_if(begin(), end(), [&key](auto key_) { return key == key_; });
    if(it == end()) {
      std::cout << "Key " << key << " not found" << '\n';
      // todo throw key error
      return;
    }
    insert_below(it, data);
  }

  auto insert_after(iterator node, T data) -> void {
    TreeNode<T>* new_node = new TreeNode<T>(data);
    if(node->first_sibling == nullptr) {
      node->first_sibling = new_node;
      return;
    }
    auto tmp = node->first_sibling;
    node->first_sibling = new_node;
    new_node->first_sibling = tmp;
  }

  auto insert_after(T key, T data) -> void {
    iterator it = std::find_if(begin(), end(), [&key](auto key_) { return key == key_; });
    if(it == end()) {
        std::cout << "Key " << key << " not found" << '\n';
        return;
    }
    insert_after(it, data);
  }

  auto append_child(iterator node, T data) -> void {
    if(node->first_child == nullptr) {
      TreeNode<T>* new_node = new TreeNode<T>(data);
      node->first_child = new_node;
      return;
    }
    auto first_child = node->first_child;
    if(first_child->first_sibling == nullptr) {
      TreeNode<T>* new_node = new TreeNode<T>(data);
      first_child->first_sibling = new_node;
      return;
    }
    auto walker = first_child;
    while(walker->first_sibling != nullptr) {
      walker = walker->first_sibling;
    }
    insert_after(walker, data);
  }

  auto append_child(T key, T data) -> void {
    iterator it = std::find_if(begin(), end(), [&key](auto key_) { return key == key_; });
    if(it == end()) {
        std::cout << "Key " << key << " not found" << '\n';
        return;
    }
    append_child(it, data);
  }

  auto remove(iterator node) -> void {
  }

  private:
  TreeNode<T>* tree;
};

template <typename Tree>
class tree_as_level_order {
  private:
    using iterator = PreOrderIterator<Tree>;
    using level_iterator = LevelOrderIterator<Tree>;
    level_iterator begin_;
    level_iterator end_;

  public:
    tree_as_level_order(const iterator& it): begin_(it.ptr_), end_(it.ptr_->first_sibling) {}
    level_iterator begin() { return begin_; }
    level_iterator end() { return end_; }
};

#endif /*ROSETREE_H*/

