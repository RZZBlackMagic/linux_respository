class Solution {
public:
    /**
     * 
     * @param arr int整型vector the array
     * @return int整型
     */
    int maxLength(vector<int>& arr) {
        vector<int> flag(100000);
        int i =0, j =0,max = 0;
        while(j<arr.size()){
            if(flag[arr[j]]==0){
                flag[arr[j]] = 1;
                max = max>j-i+1?max:j-i+1;
                j++;
            }else{
                flag[arr[i]] = 0;
                i++;
            }
        }
        return max;
        /*
        vector<int> len(100000);
        int length = 0;
        for(int i=0;i<arr.size();i++){
            vector<int> temp;
            for(int j=i;j<arr.size();j++){
                int k;
                for(k = 0;k<temp.size();k++){
                    if(temp[k]==arr[j])
                        break;
                }
                if(k==temp.size())
                    temp.push_back(arr[j]);
                else
                    break;
            }
            if(length<temp.size()){
                length = temp.size();
            }
            len.push_back(temp.size());
            if(len[i]>=arr.size()-i){
                break;
            }
        }
        return length;*/
    }
};