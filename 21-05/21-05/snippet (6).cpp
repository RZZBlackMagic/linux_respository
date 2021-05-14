class Solution {
public:
    int res;
    int jumpFloor(int number) {
        int arr[number];
        arr[0] = 1;
        arr[1] = 2;
        for(int i=2;i<number;i++){
            arr[i] = arr[i-1] + arr[i-2];
        }
        return arr[number-1];
        /*
        if(number<0){
            return 0;
        }
        if(number==0){
            return 1;
        }
        return  jumpFloor(number-1) + jumpFloor(number-2);*/
    }
};