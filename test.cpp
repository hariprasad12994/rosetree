// test file 
#include "rosetree.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// https://stackoverflow.com/questions/5973427/error-passing-xxx-as-this-argument-of-xxx-discards-qualifiers

auto test_preorder_plain_for_loop(Tree<std::string>& path_tree) -> void;
auto test_preorder_stl_for_each(Tree<std::string>& path_tree) -> void;
auto test_preorder_range_for_loop(Tree<std::string>& path_tree) -> void;
auto test_postorder_range_for_loop(Tree<std::string>& path_tree) -> void;
auto test_levelorder_range_for_loop(Tree<std::string>& path_tree) -> void;
auto test_levelorder_subtree_traversal(Tree<std::string>& path_tree) -> void;
auto test_postorder_subtree_traversal(Tree<std::string>& path_tree) -> void;
auto test_preorder_subtree_traversal(Tree<std::string>& path_tree) -> void;
auto test_subtree_delete(Tree<std::string>& path_tree) -> void;
auto test_tree_delete(Tree<std::string>& path_tree) -> void;


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

  test_preorder_plain_for_loop(path_tree);
  test_preorder_stl_for_each(path_tree);
  test_preorder_range_for_loop(path_tree);
  test_postorder_range_for_loop(path_tree);
  test_levelorder_range_for_loop(path_tree);
  test_levelorder_subtree_traversal(path_tree);
  test_postorder_subtree_traversal(path_tree);
  test_preorder_subtree_traversal(path_tree);
  test_subtree_delete(path_tree);
  test_tree_delete(path_tree);

  std::stringstream op;
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(path_tree.begin(), op);
  std::cout << op.str() << '\n';

  return 0;
}

auto test_preorder_plain_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  for(auto it = tree.begin(); it != tree.end(); it++) {
    op << *it << " ";
  }
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_preorder_stl_for_each(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  std::for_each(tree.begin(), tree.end(), [&op](auto elem) { op << elem << " "; });
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_preorder_range_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_postorder_range_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "boot/ bin/ tmp/ var/ etc/ Projects/ Documents/ hari/ admin/ usr/ opt/ / ";
  std::stringstream op;

  tree_to_sstream<std::string, tree_as_post_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_levelorder_range_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ boot/ bin/ var/ etc/ usr/ opt/ tmp/ hari/ admin/ Projects/ Documents/ ";
  std::stringstream op;

  tree_to_sstream<std::string, tree_as_level_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_preorder_subtree_traversal(Tree<std::string>& tree) -> void {
  std::string expected_op = "usr/ hari/ Projects/ Documents/ admin/ ";
  std::stringstream op;

  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("usr/"); });
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(it, op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_postorder_subtree_traversal(Tree<std::string>& tree) -> void {
  std::string expected_op = "Projects/ Documents/ hari/ admin/ usr/ ";
  std::stringstream op;

  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("usr/"); });
  tree_to_sstream<std::string, tree_as_post_order<Tree<std::string>>>(it, op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_levelorder_subtree_traversal(Tree<std::string>& tree) -> void {
  std::string expected_op = "usr/ hari/ admin/ Projects/ Documents/ ";
  std::stringstream op;

  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("usr/"); });
  tree_to_sstream<std::string, tree_as_level_order<Tree<std::string>>>(it, op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_subtree_delete(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ boot/ bin/ var/ tmp/ etc/ opt/ ";
  std::stringstream op;

  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("usr/"); });
  tree.remove(it);
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_tree_delete(Tree<std::string>& tree) -> void {
  std::string expected_op = "";
  std::stringstream op;

  tree.remove(tree.begin());
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

