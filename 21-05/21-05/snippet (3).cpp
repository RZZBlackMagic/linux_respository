/**
 * struct TreeNode {
 *	int val;
 *	struct TreeNode *left;
 *	struct TreeNode *right;
 * };
 */

class Solution {
public:
    /**
     * 
     * @param root TreeNode类 the root of binary tree
     * @return int整型vector<vector<>>
     */
    vector<vector<int> > threeOrders(TreeNode* root) {
        vector<vector<int>> res;
        vector<int> pre;
        vector<int> mid;
        vector<int> post;
        pre_print(root,pre);
        mid_print(root,mid);
        post_print(root,post);
        res.push_back(pre);
        res.push_back(mid);
        res.push_back(post);
        return res;
    };
    void pre_print(TreeNode* root,vector<int>& vec){
        if(root==nullptr)
            return ;
        vec.push_back(root->val);
        pre_print(root->left,vec);
        pre_print(root->right, vec);
    };
    void mid_print(TreeNode* root,vector<int>& vec){
        if(root==nullptr)
            return ;
        mid_print(root->left,vec);
        vec.push_back(root->val);
        mid_print(root->right, vec);
    };
    void post_print(TreeNode* root,vector<int>& vec){
        if(root==nullptr)
            return ;
        post_print(root->left,vec);
        post_print(root->right, vec);
        vec.push_back(root->val);
    };
};