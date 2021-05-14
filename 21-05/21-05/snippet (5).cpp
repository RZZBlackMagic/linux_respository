class Solution {
public:
    /**
     * 
     * @param numbers int整型vector 
     * @param target int整型 
     * @return int整型vector
     */
    vector<int> twoSum(vector<int>& numbers, int target) {
        vector<int> temp;
        temp.assign(numbers.begin(),numbers.end());
        sort(numbers.begin(),numbers.end());
        int end = numbers.size()-1;
        int begin = 0;
        while (end > begin&&numbers[begin] + numbers[end] != target) {
		    while (end > begin&&numbers[begin] + numbers[end] > target) {
			    end--;
		    }
		    while (end > begin&&numbers[begin] + numbers[end] < target) {
		         begin++;
		    }
	    }
        vector<int> res ;
        int flag = 0;
        int a ,b;
        for(int i=0;i<temp.size();i++){
            if(flag==0&&(temp[i]==numbers[begin]||temp[i]==numbers[end])){
                a = i;
                flag++;
            }else if(flag==1&&(temp[i]==numbers[begin]||temp[i]==numbers[end])){
                b = i;
                flag++;
            }
            if(flag == 2)
                break;
        }
        res.push_back(a+1);
        res.push_back(b+1);
        return res;
    }
};