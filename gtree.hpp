#include <algorithm>
#include <queue>


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
