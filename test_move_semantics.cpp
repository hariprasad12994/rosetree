#include "rosetree.hpp"
#include <iostream>

struct S{
    public:
    int a;
    std::string s;

    S(int i) { std::cout << "[DefCtor] S(int i)\n"; }
    S() { std::cout << "[DefCtor] S()\n"; }
    S(const S &obj) { std::cout << "[CopyCtor] S(const S& obj)\n"; }
    S(S&& obj) { std::cout << "[MoveCtor] S(S&& obj)\n"; } 
    ~S() { std::cout << "[Dtor] ~S()\n"; }
};

auto main(void) -> int {
  std::cout << "[Root] 1\n";
  auto another_tree = Tree(new TreeNode<S>(1));
  std::cout << "[emplace_below] 3\n";
  another_tree.emplace_below(another_tree.begin(), 3);
  std::cout << "[insert_below] 7\n";
  another_tree.insert_below(another_tree.begin(), 7);
  std::cout << "[delete_tree] tree\n";
  another_tree.remove(another_tree.begin());
  return 0;
}
