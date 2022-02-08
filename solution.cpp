#include "solution.h"

#include <unordered_map>
#include <functional>

using namespace sol1740;
using namespace std;

/*takeaways
  - this is lowest common ancestor problem
    with a twist
    - the distance is defined as follow
      distance(lca,p) + distance(lca,q)
    - the focus is to calculate the distance
      correctly not to report the lca back
    - calculate the distance on-the-fly while
      finding the lca. That way we only need
      to visit the tree once
*/

int Solution::distance(Node *root, int p, int q)
{
  /*
    - store the distance of p and q away from the lca
  */
  unordered_map<int, int> distance;

  function<Node *(Node *)> visit = [&distance, &visit, p, q](Node *node)
  {
    if (node == nullptr)
      /*
        - just to make the compiler happy
      */
      return static_cast<Node *>(nullptr);

    /* the twist
       - always visit the left and the right
         even if we already found p or q
       - this is to make sure in case if
         p is q's parent or vice versa
         that q's distance is still calcualted
    */
    auto left = visit(node->left);
    auto right = visit(node->right);

    if (node->val == p || node->val == q)
    {
      distance[node->val] = 0;
      /*
        - we are done here; the
          distance should be 0
      */
      if (p == q)
        /* exit here no point to go any further */
        return static_cast<Node *>(nullptr);

      /*
         - don't forget to count one more step
           as we are walking up the tree now
      */
      if (left != nullptr)
        distance[left->val]++;
      if (right != nullptr)
        distance[right->val]++;

      /*
        - lay say we found p, and one of its child
          is not empty. This only means one thing
          - q has been found and it's in p's subtree.
        - if this is the case we are done here. Return
          a nullptr to force the function to exit

      */
      return left == nullptr && right == nullptr ? node : static_cast<Node *>(nullptr);
    }

    if (left != nullptr && right != nullptr)
    {
      distance[left->val]++;
      distance[right->val]++;
      /*
        - p and q both have bee found;
        - exit immediately
        - our focus is to calculate the
          distance not to return the
          lca
      */
      return static_cast<Node *>(nullptr);
    }
    if (left != nullptr)
    {
      /*
        - report the left and add the distance
          by 1 as we are moving up the tree
      */
      distance[left->val]++;
      return left;
    }
    if (right != nullptr)
    {
      /*
        - same thing
      */
      distance[right->val]++;
      return right;
    }

    return static_cast<Node *>(nullptr);
  };

  visit(root);

  return distance[p] + distance[q];
}