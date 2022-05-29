# RoseTree
Rosetree is the C++ implementation for the data structure of the same name. Rosetrees are generic trees with unbounded numbers of branches at each node

Link to references about Rosetree:
* https://en.wikipedia.org/wiki/Rose_Tree
* https://cs.stackexchange.com/questions/56081/what-are-the-applications-of-rose-trees

RoseTree aims to be a header only library, compatible with the STL algorithms
like std::transform, std::find etc.

The data structure is unsorted, unbalanced tree. Users are
allowed to insert data from top to bottom and left to right

## Basic API documentation
### Tree construction

```c++
auto path_tree = Tree(new TreeNode<std::string>("/"));
```

### Tree insertion operations
Users can insert a new node in any of the three positions relative to the iterator passed as an argument.
* Below the iterator, ie as first child of the node pointed by the iterator
* Below the iterator, but as the last child of the node pointed by the iterator
* After the node pointed by the iteator

Note that only a PreOrder Depth-First iterator can be used for referencing the
positions in the insertion APIs. Refer chapter "Tree iteration" for more details
about iterators

Note that inserting into an empty tree, for example after clearing the container,
is an undefined behavior. The rationale is that the operation is done on a tree
without a root and such an operation makes no sense.

All insertion operations returns an iterator to the newly inserted node which
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
todo: Document individual APIs, with move semantics
todo: Document known bugs

### Tree deletion operations
todo: Document individual APIs
todo: Document on RAII clear

### Tree iteration
Currently supported iterators:
* Breadth-First Iterator
* PreOrder Depth-First Iterator
* PostOrder Depth-First Iterator

The default iterator on constructing a tree will be PreOrder iterator. Proxy
classes are available to convert the default iterator to other types

## Roadmap
Tentative feature road map for the Rosetree Data Structure. Please do note that
there might few features which might not be implemented and few might not be
on the list but would be implemented on the fly. List entry with "?" is some
feature which is not concretely planned
- [x] Iterators - PreOrder, PostOrder, LevelOrder
- [x] Insertion APIs - Value Semantics
- [x] Insertion APIs - Move Semantics
- [x] Insertion APIs - Emplacing / In place construction of nodes
- [x] Deletion - Erase node
- [x] Deletion - Subtree eviction by cutting
- [x] Deletion - Clear container
- [ ] Iterators - InOrder, LeafOrder
- [x] Automatic memory cleanup on destruction
- [ ] Tree copy - Deep
- [ ] Tree move
- [ ] Tree head manipulation
- [ ] Iterators - const versions
- [ ] Allocator concept for tree nodes?
- [ ] Container adapter? - Hint. For example std::priority_queue lets users to
  decide what can be the underlying container type
- [ ] Algorithms
  - [ ] Tree Merge
  - [ ] Tree Diff
  - [ ] Tree Rotate
  - [ ] Mirror

