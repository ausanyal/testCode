#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;

#define NUM_HOUSES 5
#define MAX_WEALTH_PER_HOME 100

 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };

typedef struct TreeNode node;
//typedef pair<int, int> pair<int, int>;
int gen_rand(int modulo);

class Solution {
public:
    pair<int, int> dfs(TreeNode *root);
    int rob(TreeNode* root);
    void make_house(node **root, int wealth);
};


// Rob(root): max(dont_rob(l) + dont_rob(r) + root.val, rob(l) + rob(r))
// Dont_Rob(root): rob(l) + rob(r)
pair<int, int>
Solution::dfs(TreeNode *root)
{
  pair<int, int> dp = make_pair(0, 0);
  pair<int, int> dp_l, dp_r;
  if (root) {
    dp_l = this->dfs(root->left);
    dp_r = this->dfs(root->right);
    // second marks dont rob
    // first marks rob
    dp.second = dp_l.first + dp_r.first;
    dp.first  = max(dp_l.second + dp_r.second + root->val, dp.second);
  }
  return dp;
}

int
Solution::rob(TreeNode *root)
{
  return this->dfs(root).first;
}

int main()
{
    Solution s;
    node *root = NULL;

    for (int i = 0; i < NUM_HOUSES; i++) {

      s.make_house(&root, gen_rand(MAX_WEALTH_PER_HOME));
    }

    int r = s.rob(root);
    cout << "Max wealth_accum: \n" << r << endl;

    return 0;
}

void
Solution::make_house(node **root, int wealth)
{
  static int count = 0;
  node *nnode = NULL;

  if (*root == NULL) {
    *root = new node(wealth);
    cout << "node: " << count <<
            ". Wealth: " << (*root)->val << endl;
    count++;
  } else {
    cout << "Root filled. Going to ..";
    if(gen_rand(2) == 0) { //go left
      cout << " left child\n";
      this->make_house(&(*root)->left, wealth);
    } else {  // go right
      cout << " right child\n";
      this->make_house(&(*root)->right, wealth);
    }
  }

  return;
}

int gen_rand(int modulo)
{
  static int seed_rand = 0;

  if (!seed_rand) {
    //init the random seed
    srand(time(NULL));
    seed_rand = 1;
  }

  //get random number % modulo
  int r = rand();

  //cout << "r: " << r << endl;

  return (r % modulo);
}
