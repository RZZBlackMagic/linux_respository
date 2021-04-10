#include<iostream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
	string arr;
//        istringstream = iss(arr);
        vector<string> res_arr;
int fun(string split,istringstream iss){
	string token ;
	while(getline(iss,token,split)){
		res_arr.push_back(token);
	}
        if(res_arr.pop_back()!=""){
                for(int i=0;i<res_arr.size();i++){
                        if(res_arr.at(i).find("0",0)==0){
                                cout<<"false"<<endl;
				return 0;
                                exit(0);
                        }
                }
        }else{
		cout<<"false"<<endl;
		exit(0);
	}
	return 1;
}

int main(){
	cin>>arr;
	istringstream iss(arr);
	if(fun(":",iss)){
		for(int i=0;i<res_arr.size();i++){
			if(res_arr.at(i)==""){
				cout<<"false"<<endl;
				exit(0);
			}
			for(int j=0;j<res_arr.at(j).length();j++){
				if(!(res_arr.at(i)[j] == "[a-z]"||res_arr.at(i)[j]=="[A-Z]"||res_arr.at(i)[j]=="[0-9]")){
					cout<<"false"<<endl;
					exit(0);
				}
			}
		}
		cout<<"IPv6"<<endl;
	}else if(fun(".",iss)){
		for(int i=0;i<res_arr.size();i++){
			if(res_arr.at(i)==""){
				cout<<"false"<<endl;
				exit(0);
			}
			for(int j=0;j<res_arr.at(j).length();j++){
				if(!(res_arr.at(i)[j]=="[0-9]")){
					cout<<"false"<<endl;
					exit(0);
				}
			}
		}
		cout<<"IPv4"<<endl;
	}else{
		cout<<false<<endl;
		exit(0);
	}
	return 0;
}
