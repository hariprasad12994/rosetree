# RoseTree
Rosetree is the C++ implementation for the data structure of the same name. Rosetrees are generic trees with unbounded number of branches at each node

Link to references about Rosetree:
* https://en.wikipedia.org/wiki/Rose_Tree
* https://cs.stackexchange.com/questions/56081/what-are-the-applications-of-rose-trees

RoseTree aims to be a header only library, compatible with the STL algorithms
like std::transform, std::find etc.

The data structure is unsorted, unbalanced tree. Users are
allowed to insert data from top to bottom and left to right

Users are free to use the single header rosetree.hpp in the project
The container works best with C++14 and above

## Basic API documentation
### Rosetree
```c++
template<
  class T
> class Tree;
```
#### Member types
<pre>
iterator                 PreOrder iterator for the tree container
</pre>
#### Member functions
<pre>
(constructor)            Constructs the rosetree
(destructor)             Destructs the rosetree
operator=                Assigns values to the container

begin                    Returns an iterator to the beginning
end                      Returns an iterator to the end

insert_below      
insert_after
append_child             Inserts elements with respect to a pre-order iterator

emplace_below
emplace_after
append_child_by_emplace  Constructs an element in-place at a position with respect to a preorder iterator

cut                      Evict a subtree out of the container as a new container
erase                    Delete a subtree pointed by iterator
clear                    Clears the contents

empty                    Checks if container is empty
</pre>

### Tree construction
Like every other STL container RoseTree can be templated with the data item to
be held
```c++
// 1
template <typename ... Args>
Tree(Args ... args);

// 2
Tree(const Tree& other);
Tree& operator=(const Tree& other);

// 3
Tree(Tree&& other);
Tree& operator=(Tree&& other);
```

Detailed illustration
```c++
// Construct tree with root element containing data "/"
Tree<std::string> path_tree(std::string("/"));

// Its ensured that the tree always contains a root. 
// Below line causes compilation error stating usage of deleted function
Tree<std::string> tree_without_root;

// Copy constructor. 
// Creates a deep copy of path tree into another_path_tree
Tree<std::string> another_path_tree(path_tree);

// Move constructor.
// Moves path_tree into yet_another_tree and leave path_tree in a valid but undefined state
Tree<std::string> yet_another_tree = std::move(path_tree);
// Below line is classic use after move
// Causes undefined behavior
path_tree.insert(path_tree.begin(), "/");
```

### Tree insertion operations
Users can insert a new node in any of the three positions relative to the iterator passed as an argument.
* Below the iterator, as the first child of the node pointed by the iterator
  ```c++
  auto insert_below(iterator node, const T& data) -> iterator;
  auto insert_below(iterator node, T&& data) -> iterator;
  template <typename ... Args>
  auto emplace_below(iterator node, Args ... args) -> iterator;
  ```
  Detailed illustration
  ```c++
  Tree<std::string> tree("root");
  auto it = tree.begin();

  // Insert by value semantics
  tree.insert_below(it, std::string("hello"));
  
  // Insert by move semantics
  std::string some_string = "world";
  tree.insert_below(it, std::move(some_string));

  struct S {
    int int_mem;
    float flt_mem;
    std::string str_mem;
  };
  Tree<S> s_tree(2, 10.0f, std::string("root"));
  // In place construction of node below root
  s_tree.emplace_below(s_tree.begin(), 1, 5.0f, "42");
  ```

* Below the iterator, as the last child of the node pointed by the iterator
  ```c++
  auto append_child(iterator node, const T& data) -> iterator;
  auto append_child(iterator node, T&& data) -> iterator;
  template <typename ... Args>
  auto append_child_by_emplace(iterator node, Args ... args) -> iterator;
  ```
  Detailed illustration
  ```c++
  Tree<std::string> tree("root");
  auto it = tree.begin();

  // Insert by value semantics
  tree.append_child(it, std::string("hello"));
  
  // Insert by move semantics
  std::string some_string = "world";
  tree.append_child(it, std::move(some_string));

  struct S {
    int int_mem;
    float flt_mem;
    std::string str_mem;
  };
  Tree<S> s_tree(2, 10.0f, std::string("root"));
  // In place construction of node below root
  s_tree.append_child_by_emplace(s_tree.begin(), 1, 5.0f, "42");
  ```
  
* After the iterator, as the sibling of the node pointed by the iterator
  ```c++
  auto insert_after(iterator node, const T& data) -> iterator;
  auto insert_after(iterator node, T&& data) -> iterator;
  template <typename ... Args>
  auto emplace_after(iterator node, Args ... args) -> iterator;
  ```
  Detailed illustration
  ```c++
  Tree<std::string> tree("root");
  auto it = tree.begin();

  // Insert by value semantics
  tree.insert_after(it, std::string("hello"));
  
  // Insert by move semantics
  std::string some_string = "world";
  tree.insert_after(it, std::move(some_string));

  struct S {
    int int_mem;
    float flt_mem;
    std::string str_mem;
  };
  Tree<S> s_tree(2, 10.0f, std::string("root"));
  // In place construction of node below root
  s_tree.emplace_after(s_tree.begin(), 1, 5.0f, "42");
  ```

#### Notes
* Only a PreOrder Depth-First iterator can be used for referencing the
positions in the insertion APIs. Refer chapter "Tree iteration" for more details
about iterators
* Inserting into an empty tree, for example after clearing the container,
is an undefined behavior. The rationale is that the operation is done on a tree without a root and such an operation makes no sense.
  ```c++
  Tree<int> integer_tree(1);
  integer_tree.clear();
  // Undefined behavior
  integer_tree.insert_below(integer_tree.begin(), 42);

  // Alternative
  Tree<int> integer_tree(1);
  integer_tree.clear();
  Tree<int> new_integer_tree(42);
  // Copy the new tree into the original tree. Also it is fine to move the tree
  // by marking it with std::move
  integer_tree = new_integer_tree;
  ```
* All insertion operations returns an iterator to the newly inserted node which
can be discarded if not required.
  ```c++
  // Stores the returned iterator for further operations
  auto boot = path_tree.insert_below(path_tree.begin(), std::string("boot/"));
  auto bin = path_tree.append_child(path_tree.begin(), std::string("bin/"));
  auto var = path_tree.append_child(path_tree.begin(), std::string("var/"));
  auto usr = path_tree.append_child(path_tree.begin(), std::string("usr/"));
  // It is also ok to discard the returned iterator
  path_tree.insert_below(usr, std::string("hari/"));
  ```
todo: Document known bugs

### Tree deletion operations
There are 3 APIs to support deletion operations
* cut<br/>
  This enables cutting a subtree out of the container and create a new tree
  container with the node evicted as the root
  ```c++
  auto cut(iterator node) -> Tree<T>;
  ```
  Example
  ```c++
  auto spliced_tree = tree.cut(it);
  ```
* erase<br/>
  Deletes a node and all the children below it from the tree container. Requires
  cautious usage
  ```c++
  auto erase(iterator node) -> void;
  ```
  Example
  ```c++
  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("etc/"); });
  tree.erase(it);
  ```

* clear<br/>
  Clears the complete container
  ```c++
  auto clear() -> void;
  ```
  Example
  ```c++
  tree.clear();
  // Undefined behavior. Insertion into a tree without root
  tree.insert_below(tree.begin(), std::string("42"));
  ```

#### RAII
The container like every other STL container is cleaned up automatically when it
goes out of scope

### Tree iteration
Currently supported iterators:
* Breadth-First Iterator
* PreOrder Depth-First Iterator
* PostOrder Depth-First Iterator

The default iterator on constructing a tree will be PreOrder iterator. Proxy
classes are available to convert the default iterator to other types
```c++
template <typename Tree>
class tree_as_level_order;

template<typename Tree>
class tree_as_pre_order;

template <typename Tree>
class tree_as_post_order;
```

### Tree querying

* empty <\b>
  Checks if the container is empty. ie if the tree has no nodes in it

  ```c++
  auto empty() -> bool;
  ````
  Example
  ```c++
  auto is_empty = tree.empty();
  if(!is_empty)
    return true;
  ```

## Roadmap
Tentative feature road map for the Rosetree Data Structure. Please do note that
there might few features which might not be implemented and few might not be
on the list but would be implemented on the fly. List entry with "?" is some
feature which is not concretely planned
- [x] Iterators - PreOrder, PostOrder, LevelOrder
- [x] Insertion APIs - Value Semantics
- [x] Insertion APIs - Move Semantics
- [x] Insertion APIs - Emplacing / In place construction of nodes
- [ ] Deletion - Erase node
- [x] Deletion - Erase subtree
- [x] Deletion - Subtree eviction by cutting
- [x] Deletion - Clear container
- [ ] Iterators - InOrder, LeafOrder
- [x] Automatic memory cleanup on destruction
- [x] Tree copy - Deep
- [x] Tree move
- [ ] Code cleanups for readability
- [ ] Code cleanups for better exposed public and private API
- [ ] Optimization - const and const& as much as possible
- [ ] Traverals with callbacks
- [x] API to check if container is empty
- [ ] Iterators - const versions
- [ ] Allocator concept for tree nodes?
- [ ] Container adapter? - Hint. For example std::priority_queue lets users to
  decide what can be the underlying container type
- [ ] Optimization - Cache friendliness
- [ ] Algorithms
  - [ ] Tree Merge
  - [ ] Tree Diff
  - [ ] Serialization and Deserialization
  - [ ] Path Manipulation
  - [ ] Tree Rotate
  - [ ] Mirror

