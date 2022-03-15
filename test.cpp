// test file
#include "rosetree.hpp"
#include <iostream>
#include <string>

auto main(void) -> int {
  auto path_tree = Tree(new TreeNode<std::string>("/"));
  
  path_tree.insert_below(path_tree.begin(), std::string("boot/"));
  path_tree.append_child(path_tree.begin(), std::string("bin/"));
  path_tree.append_child(path_tree.begin(), std::string("var/"));
  path_tree.append_child(path_tree.begin(), std::string("usr/"));
  path_tree.insert_below(std::string("var/"), std::string("tmp/"));
  path_tree.append_child(std::string("/"), std::string("opt/"));
  path_tree.insert_after(std::string("var/"), std::string("etc/"));
  path_tree.insert_below(std::string("usr/"), std::string("hari/"));
  path_tree.insert_below(std::string("hari/"), std::string("Documents/"));
  path_tree.insert_below(std::string("hari/"), std::string("Projects/"));
  path_tree.append_child(std::string("usr/"), std::string("admin/"));

  // plain iterator based for loop
  for(auto it = path_tree.begin(); it != path_tree.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << '\n';

  // stl for_each based iteration
  std::for_each(path_tree.begin(), path_tree.end(), [](auto elem) { std::cout << elem << " "; });
  std::cout << '\n';

  // range based for-loop
  for(auto tree_elem: path_tree) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';

  for(auto tree_elem: tree_as_level_order(path_tree.begin())) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';

  for(auto tree_elem: tree_as_post_order(path_tree.begin())) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';

  for(auto tree_elem: tree_as_pre_order(path_tree.begin())) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';

  auto it = std::find_if(path_tree.begin(), path_tree.end(), [](auto elem){ return elem == std::string("usr/"); });
  for(auto tree_elem: tree_as_pre_order(it)) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';
  for(auto tree_elem: tree_as_post_order(it)) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';
  for(auto tree_elem: tree_as_level_order(it)) {
    std::cout << tree_elem << " ";
  }
  std::cout << '\n';

  return 0;
}
