# RoseTree
A generic tree data structure written in C++

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


