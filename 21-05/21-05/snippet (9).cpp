/**
 * struct ListNode {
 *	int val;
 *	struct ListNode *next;
 * };
 */

class Solution {
public:
    /**
     *.
     * @param head ListNode类 
     * @param k int整型 
     * @return ListNode类
     */
    ListNode* reverse(ListNode* head,int k){
        ListNode* pre = head->next;
        ListNode* cur = pre->next;
        while(k>1){
            pre->next = cur->next;
            cur->next = head->next;
            head->next = cur;
            pre=pre->next;
            cur= cur->next;
            k--;
        }
        return pre;
    };
    inline bool is_empty(ListNode* _head,int k){
        while(_head->next!=nullptr&&k>0){
            _head = _head->next;
            k--;
        }
        return k==0?true:false;
    }
    ListNode* reverseKGroup(ListNode* head, int k) {
        if(head==nullptr)
            return head;
        ListNode* _head = (ListNode*)malloc(sizeof(ListNode));
        _head->next = head;
        ListNode* temp_head = _head;
        while(temp_head!=nullptr&&is_empty(temp_head, k)){
            temp_head = reverse(_head,  k);
        }
        return _head->next;
    }
};