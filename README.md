# RoseTree
Rosetree is the C++ implementation for the data structure of the same name. Rosetrees are generic trees with unbounded numbers of branches at each node

Link to references about Rosetree:
* https://en.wikipedia.org/wiki/Rose_Tree
* https://cs.stackexchange.com/questions/56081/what-are-the-applications-of-rose-trees

RoseTree aims to be a header only library, compatible with the STL algorithms
like std::transform, std::find etc.

The data structure is unsorted, unbalanced tree with unique keys. Users are
allowed to insert data from top to bottom and left to right

Currently supported iterators:
* Breadth-First Iterator
* PreOrder Depth-First Iterator
* PostOrder Depth-First Iterator

The default iterator on constructing a tree will be PreOrder iterator. Proxy
classes are available to convert the default iterator to other types
todo: add code examples for iterator conversion


