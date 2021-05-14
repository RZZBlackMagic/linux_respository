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
     * @param root TreeNode类 
     * @return int整型vector<vector<>>
     */
    vector<vector<int>> levelOrder(TreeNode* root) {
        // 层次遍历
        vector<vector<int> > res;
        TreeNode* m_right = root;
        queue<TreeNode*> que;
        que.push(root);
        vector<int> temp;
        while(!que.empty()){
            TreeNode* cur = que.front();
            que.pop();
            if(cur->left!=nullptr)
                que.push(cur->left);
            if(cur->right!=nullptr)
                que.push(cur->right);
            if(cur==m_right){
                temp.push_back(cur->val);
                res.push_back(temp);
                m_right = que.back();
                temp.clear();
            }else{
                temp.push_back(cur->val);
            }
        }
        return res;
    }
};