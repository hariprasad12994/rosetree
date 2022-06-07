// test file 
#include "../rosetree.hpp"
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
auto test_tree_copy(Tree<std::string>& path_tree) -> void;
auto test_tree_move(Tree<std::string>& path_tree) -> void;
auto test_tree_node_delete(Tree<std::string>& path_tree) -> void;
auto test_subtree_delete(Tree<std::string>& path_tree) -> void;
auto test_tree_delete(Tree<std::string>& path_tree) -> void;


auto main(void) -> int {
  Tree<std::string> path_tree(std::string("/"));
  
  auto boot = path_tree.insert_below(path_tree.begin(), std::string("boot/"));
  auto bin = path_tree.append_child(path_tree.begin(), std::string("bin/"));
  auto var = path_tree.append_child(path_tree.begin(), std::string("var/"));
  auto usr = path_tree.append_child(path_tree.begin(), std::string("usr/"));
  auto tmp = path_tree.insert_below(var, std::string("tmp/"));
  path_tree.append_child(path_tree.begin(), std::string("opt/"));
  path_tree.insert_after(var, std::string("etc/"));
  auto hari = path_tree.insert_below(usr, std::string("hari/"));
  path_tree.insert_below(hari, std::string("Documents/"));
  path_tree.insert_below(hari, std::string("Projects/"));
  path_tree.append_child(usr, std::string("admin/"));
  auto arch = path_tree.emplace_below(path_tree.begin(), "arch");
  path_tree.emplace_below(arch, "arm64");

  test_preorder_plain_for_loop(path_tree);
  test_preorder_stl_for_each(path_tree);
  test_preorder_range_for_loop(path_tree);
  test_postorder_range_for_loop(path_tree);
  test_levelorder_range_for_loop(path_tree);
  test_levelorder_subtree_traversal(path_tree);
  test_postorder_subtree_traversal(path_tree);
  test_preorder_subtree_traversal(path_tree);
  test_tree_copy(path_tree);
  test_tree_move(path_tree);
  test_tree_node_delete(path_tree);
  test_subtree_delete(path_tree);
  test_tree_delete(path_tree);

  return 0;
}

auto test_preorder_plain_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  for(auto it = tree.begin(); it != tree.end(); it++) {
    op << *it << " ";
  }
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_preorder_stl_for_each(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  std::for_each(tree.begin(), tree.end(), [&op](auto elem) { op << elem << " "; });
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_preorder_range_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_postorder_range_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "arm64 arch boot/ bin/ tmp/ var/ etc/ Projects/ Documents/ hari/ admin/ usr/ opt/ / ";
  std::stringstream op;

  tree_to_sstream<std::string, tree_as_post_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_levelorder_range_for_loop(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch boot/ bin/ var/ etc/ usr/ opt/ arm64 tmp/ hari/ admin/ Projects/ Documents/ ";
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

auto test_tree_copy(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op_1;
  std::stringstream op_2;
  std::stringstream op_3;

  Tree tree_copy(tree);
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree_copy.begin(), op_1);
  std::cout << op_1.str() << '\n';
  assert(op_1.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';

  Tree tree_another_copy = tree;
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree_another_copy.begin(), op_2);
  std::cout << op_2.str() << '\n';
  assert(op_2.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';

  Tree<std::string> yet_another_copy("42");
  yet_another_copy = tree;
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(yet_another_copy.begin(), op_3);
  std::cout << op_3.str() << '\n';
  assert(op_3.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_tree_move(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ etc/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op_1;
  std::stringstream op_2;

  Tree tree_copy(tree);
  Tree tree_by_move = std::move(tree_copy);
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree_by_move.begin(), op_1);
  std::cout << op_1.str() << '\n';
  assert(op_1.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';

  Tree another_tree_copy(tree);
  Tree<std::string> another_tree_by_move("42");
  another_tree_by_move = std::move(another_tree_copy);
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(another_tree_by_move.begin(), op_2);
  std::cout << op_2.str() << '\n';
  assert(op_2.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_tree_node_delete(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ usr/ hari/ Projects/ Documents/ admin/ opt/ ";
  std::stringstream op;

  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("etc/"); });
  tree.erase(it);
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_subtree_delete(Tree<std::string>& tree) -> void {
  std::string expected_op = "/ arch arm64 boot/ bin/ var/ tmp/ opt/ ";
  std::stringstream op;

  auto it = std::find_if(tree.begin(), tree.end(), [](auto elem){ return elem == std::string("usr/"); });
  tree.erase(it);
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

auto test_tree_delete(Tree<std::string>& tree) -> void {
  std::string expected_op = "";
  std::stringstream op;

  tree.clear();
  tree_to_sstream<std::string, tree_as_pre_order<Tree<std::string>>>(tree.begin(), op);
  assert(op.str() == expected_op);
  std::cout << "[PASSED] " << __func__ << '\n';
}

