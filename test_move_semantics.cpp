#include "rosetree.hpp"
#include <iostream>

struct MyStruct{
    public:
    int i;
    std::string s;

    MyStruct(int i): i(i), s("empty") { std::cout << "[Ctor] MyStruct(int i)\n"; }
    MyStruct(std::string str): i(-1), s(str) { std::cout << "[Ctor] MyStruct(std:string str)\n"; }
    MyStruct(int i, std::string str): i(i), s(str) { std::cout << "[Ctor] MyStruct(int i, std::string str)\n"; }
    MyStruct(): i(0), s("empty") { std::cout << "[DefCtor] MyStruct()\n"; }
    MyStruct(const MyStruct &obj) { std::cout << "[CopyCtor] MyStruct(const MyStruct& obj)\n"; i = obj.i; s = obj.s; }
    MyStruct(MyStruct&& obj) { std::cout << "[MoveCtor] MyStruct(MyStruct&& obj)\n"; i = obj.i; s = std::move(obj.s); obj.i = 0; obj.s = "empty"; } 
    ~MyStruct() { std::cout << "[Dtor] for " << i << " " << s << "\n"; }
    friend bool operator==(const MyStruct& lhs, const MyStruct& rhs) { return lhs.i == rhs.i; }
};


auto main(void) -> int {
  std::cout << "[Root] 1\n";
  auto another_tree = Tree(new TreeNode<MyStruct>(1));
  std::cout << '\n';

  std::cout << "[emplace_below -> 1] 3, hello\n";
  another_tree.emplace_below(another_tree.begin(), 3, std::string("hello"));
  std::cout << '\n';

  std::cout << "[insert_below -> 1] 7, seven\n";
  MyStruct s_7 = MyStruct(7, "seven");
  auto s_7_it = another_tree.insert_below(another_tree.begin(), s_7);
  std::cout << '\n';

  std::cout << "[insert_below -> 7] 9, nine\n";
  MyStruct s_9 = MyStruct(9, "nine");
  another_tree.insert_below(s_7_it, s_9);
  std::cout << '\n';

  std::cout << "[emplace_below -> 7] 11\n";
  another_tree.emplace_below(s_7_it, 11);
  std::cout << '\n';

  std::cout << "[emplace_below -> 7] 66\n";
  another_tree.insert_below(s_7_it, std::string("hello"));
  std::cout << '\n';

  for(auto elem: tree_as_pre_order(another_tree.begin())) std::cout << elem.i << ", " << elem.s << "| ";
  std::cout << '\n';

  std::cout << "[insert_below -> 1] 0, nothing\n";
  MyStruct s = MyStruct(0, "nothing");
  another_tree.insert_below(another_tree.begin(), s);
  std::cout << '\n';

  std::cout << "[insert_below -> 1] rvalue MyStruct(15, world)\n";
  another_tree.insert_below(another_tree.begin(), MyStruct(15, "world"));
  std::cout << '\n';

  std::cout << "[emplace below -> 1] rvalue MyStruct(17, seventeen)\n";
  another_tree.emplace_below(another_tree.begin(), 17, "seventeeen");
  std::cout << '\n';

  // check if mutating original data modifies container values which were
  // inserted by copy
  s_7.s = "new seven";
  for(auto elem: another_tree) std::cout << elem.i << ", " << elem.s << "| ";
  std::cout << '\n';

  std::cout << "[delete_tree] tree\n";
  another_tree.erase(another_tree.begin());
  return 0;
}
