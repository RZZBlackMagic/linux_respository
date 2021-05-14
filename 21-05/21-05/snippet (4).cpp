#include<vector>
#include<deque>
class Solution {
public:
    /**
     * lru design
     * @param operators int整型vector<vector<>> the ops
     * @param k int整型 the k
     * @return int整型vector
     */
    //int arr[100000] = {-1};
    map<int,int> arr;
    vector<int> lru;
    void set(long key,long value,int k){
        arr[key] = value;
        for(int i = 0 ;i<lru.size();i++){
            if(key==lru[i]){
                lru.erase(i+lru.begin());
                break;
            }
        }
        if(lru.size()>=k){
            lru.erase(lru.begin());
        }
        lru.push_back(key);
    }
    long get(long key){
         for(int i = 0 ;i<lru.size();i++){
            if(key==lru[i]){
                lru.erase(i+lru.begin());
                lru.push_back(key);
                return arr[key];
            }
        }
        return -1;
    }
    vector<int> LRU(vector<vector<int> >& operators, int k) {
        vector<int> res;
        for(int i=0;i<operators.size();i++){
            if(operators[i][0]==1){
                //set
                set(operators[i][1],operators[i][2],k);
            }else if(operators[i][0]==2){
                //get
                res.push_back(get(operators[i][1]));
            }else{
                assert(false);
            }
        }
        return res;
    }
};