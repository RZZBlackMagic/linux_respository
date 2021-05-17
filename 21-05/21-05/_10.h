#include<vector>
#include<deque>
#include<assert.h>
#include<algorithm>
#include<map>
using namespace std;
void quick_sort(int* arr,int n) {
	if (n <= 1) {
		return;
	}
	int begin = 1;
	int end = n - 1;
	int temp;
	while (begin < end) {
		while (begin < n - 1 && arr[begin] <= arr[0]) {
			begin++;
		}
		cout << "begin:" << begin << endl;
		while (end > 0 && arr[end] > arr[0]) {
			end--;
		}
		cout << "end:" << end << endl;
		if (end > begin) {
			temp = arr[end];
			arr[end] = arr[begin];
			arr[begin] = temp;
		}
		if(end <= begin){
			temp = arr[end];
			arr[end] = arr[0];
			arr[0] = temp;
		}
	}
	for (int i = 0; i < n; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	cout << "end:" << end << endl;
	cout << "begin:" << begin << endl;
	quick_sort(arr, end);
	quick_sort(arr + begin , (n - end - 1));
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << arr[i] << " ";
	}
}

void ISBN() {
	int N;
	cin >> N;
	if (N <= 0) {
		return;
	}
	int index;
	int arr[1000] = { 0 };
	for (int i = 0; i < N; i++) {
		cin >> index;
		if (index < 0 || index>1000) {
			return;
		}
		arr[index]++;
	}
	int num = 0;
	for (int i = 0; i < 1000; i++) {
		if (arr[i]) {
			num++;
			cout << i << " ";
		}
	}
	cout << endl;
	cout << num << endl;
}

void test() {
	int arr[10] = { 6,2,5,5,4,5,6,3,7,6 };
	int arr1[8] = { 0,0,1,1,1,3,3,1 };
	int M;
	cin >> M;
	M -= 4;
	for (int i = 2; i < 8; i++) {
		int A = arr1[i];
		
	}
}
struct point {
	int x;
	int y;
	point() {
		this->x = 0;
		this->y = 0;
	};
	point(int x,int y) {
		this->x = x;
		this->y = y;
	}
	point(point& p) {
		this->x = p.x;
		this->y = p.y;
	}
	bool operator==(const point& p1) const {
		if (this->x == p1.x&&this->y == p1.y) {
			return true;
		}
		else
			return false;
	}
	bool operator<(const point& p) const{
		return this->x < p.x;
	}
};
class myCompare {
public:
	bool operator()(const point& p1,const point& p2) {
		return p1.x > p2.x;
	}
	
};
/*
void deal(vector<vector<point>>& v) {
//	vector<point> max_vec;
	vector<point> temp;
	int max_num;
	for (auto it = v.begin(); it != v.end(); it++) {
		vector<point> cur = *it;
		if (it == v.begin()) {
			for (auto cur_it = cur.begin(); cur_it != cur.end(); cur_it++) {
				max_vec.push_back(*cur_it);
			}
			max_num = 1;
			break;
		}
		for (auto cur_it = cur.begin(); cur_it != cur.end(); cur_it++) {
			for (auto max_it = max_vec.begin(); max_it != max_vec.end(); max_it++) {
				if (cur_it->x == max_it->x&&cur_it->y==max_it->y) {
					temp.push_back(*cur_it);

				}
			}
		}
	}
}

void test1() {
	vector<point> max_vec;
	vector<vector<point>> v;
	int N;
	while (N--) {
		int M;
		cin >> M;
		for (int i = 0; i < M; i++) {
			int n;
			cin >> n;
			vector<point> vec(n);
			for (int j = 0; j < n; j++) {
				point p;
				cin >> p.x;
				cin >> p.y;
				if (i == 0) {
					max_vec.push_back(p);
				}
				vec.push_back(p);
			}
			v.push_back(vec);
		}
		//deal(v);
	}
}
*/
void test2() {
	map<pair<int,int>,int> m;
	map<pair<int,int>, int> max;
	int max_num = 0;
	int N;
	cin >> N;
	while (N--) {
		int M;
		cin >> M;
		for (int i = 0; i < M; i++) {
			int n;
			cin >> n;
			for (int j = 0; j < n; j++) {
				pair<int,int> p;
				cin >> p.first;
				cin >> p.second;
				if (m.count(p)) {
					max[p] = m[p] + 1;
				}
				else {
					max[p] = 1;
				}
				if (max_num < max[p]) {
					max_num = max[p];
				}
			}
			m.clear();
			m.swap(max);
		}
	}
	cout << max_num << endl;
}

vector<int> MySort(vector<int>& arr) {
	cout << "start:" << arr.size() << endl;
	if (arr.size() <= 1) {
		return arr;
	}
	int size = arr.size();
	int begin = 1;
	int end = size - 1;
	while (begin < end) {
		cout << "begin:" << begin << endl;
		cout << "end:" << end << endl;
		while (begin < size - 1 && arr.at(begin) <= arr.at(0)) {
			begin++;
		}
		while (end > 0 && arr.at(end) > arr.at(0)) {
			end--;
		}
		if (begin < end) {
			int temp = arr.at(end);
			arr.at(end) = arr.at(begin);
			arr.at(begin) = temp;
		}
		else {
			int temp = arr.at(end);
			arr.at(end) = arr.at(0);
			arr.at(0) = temp;
		}
	}
	int i = 0;
	vector<int>::iterator it = arr.begin();
	while (i < end) {
		i++;
		it++;
	}
	vector<int> left(arr.begin(), it);
	vector<int> right(it + 1, arr.end());
	left = MySort(left);
	right = MySort(right);
	arr.erase(arr.begin(), it);
	arr.insert(arr.begin(), left.begin(), left.end());
	arr.erase(it + 1, arr.end());
	arr.insert(it + 1, right.begin(), right.end());
	return arr;
}

void test3() {
	vector<int> vec = {5,2,3,1,4};
	for (auto it = vec.begin(); it != vec.end(); it++) {
		cout << *it << " ";
	}
	vec = MySort(vec);
	for (auto it = vec.begin(); it != vec.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
/*
long arr[100000] = { -1 };
int set(long key, long value) {
	arr[key] = value;
	return key;
}
long get(long key) {
	return arr[key];
}

vector<int> LRU1(vector<vector<int> >& operators, int k) {
	deque<vector<int>> q;
	vector<int> res;
	cout << "A" << endl;
	cout << operators.size() << endl;
	for (auto it = operators.begin(); it != operators.end(); it++) {
		vector<int> cur = *it;
		vector<int> q_vec;
		cout << "cur.front:" << cur.front() << endl;
		if (cur.front() == 2) {
			//get
			cout << "get" << endl;
			cout << "cur.size:" << cur.size() << endl;
			if (cur.size() >= 2) {
				long value;
				bool flag = true;
				if (q.size() > 0) {
					cout << "q.size:" << q.size() << endl;
					for (auto ite = q.begin(); ite != q.end(); ite++) {
						if ((*ite).at(0) == cur.at(1)) {
							flag = false;
							value = (*ite).at(1);
							cout << "key:" << cur.at(1) << endl;
							q.erase(ite);
							cout << "key:" << cur.at(1) << endl;
							//arr[*ite->begin()] = -1;
							break;
						}
					}
				}
				else {
					cout << "." << endl;
					assert(false);
				}

				if (flag) {
					value = -1;
				}
				cout << "?" << endl;

				res.push_back(value);
				q_vec.push_back(cur.at(1));
				q_vec.push_back(value);
				cout << ":" << endl;
				if (q.size() == k) {
					//    vector<int> temp = q.front();
					q.pop_front();
					//arr[temp.front()] = -1;
				}
				q.push_back(q_vec);
			}
			else {
				cout << "<" << endl;
				assert(false);
			}
		}
		else if (cur.front() == 1) {
			//set
			cout << "set" << endl;

			if (cur.size() >= 3) {
				set(cur.at(1), cur.at(2));
				q_vec.push_back(cur.at(1));
				q_vec.push_back(cur.at(2));
				if (q.size() == k) {
					//    vector<int> temp = q.front();
					q.pop_front();
					//arr[temp.front()] = -1;
				}
				q.push_back(q_vec);
			}
		}
		else {
			cout << "error" << endl;

		}
	}
	return res;
}
*/
int arr[100000] = { -1 };
vector<int> lru;
void set(long key, long value, int k) {
	arr[key] = value;
	for (int i = 0; i < lru.size(); i++) {
		if (key == lru[i]) {
			lru.erase(i + lru.begin());
			break;
		}
	}
	if (lru.size() >= k) {
		lru.erase(lru.begin());
	}
	lru.push_back(key);
}
long get(long key) {
	for (int i = 0; i < lru.size(); i++) {
		if (key == lru[i]) {
			lru.erase(i + lru.begin());
			lru.push_back(key);
			return arr[key];
		}
	}
	return -1;
}
vector<int> LRU(vector<vector<int> >& operators, int k) {
	vector<int> res;
	for (int i = 0; i < operators.size(); i++) {
		if (operators[i][0] == 1) {
			//set
			set(operators[i][1], operators[i][2], k);
		}
		else if (operators[i][0] == 2) {
			//get
			res.push_back(get(operators[i][1]));
		}
		else {
			assert(false);
		}
	}
	return res;
}
//[[1,1,1],[1,2,2],[1,3,2],[2,1],[1,4,4],[2,2]],3
void test4() {
	vector<vector<int>> vec;
	for (int i = 0; i < 6 ; i++ ) {
		vector<int> temp;
		int flag;
		cin >> flag;
		if (flag == 1) {
			temp.push_back(flag);
			for (int i = 0; i < 2; i++) {
				cin >> flag;
				temp.push_back(flag);
			}
		}
		else {
			temp.push_back(flag);	
			cin >> flag;
			temp.push_back(flag);
		}
		vec.push_back(temp);
	}
	int k;
	cin >> k;
	vector<int> res = LRU(vec, k);
	for (auto it = res.begin(); it != res.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
/*
 vector<vector<int>> levelOrder(TreeNode* root) {
        // ²ã´Î±éÀú
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
	*/
void quickSort(vector<int>& vec, int begin, int end, int K, int size) {
	if (vec.size() <= 1 || begin > end || size <= 1) {
		return;
	}
	int monitor = begin;
	//begin++;
	while (begin < end) {
		cout << "begin:" << begin << endl;
		cout << "end:" << end << endl;
		cout << "mon:" << vec[monitor] << endl;
		while (begin < size - 1 && vec[begin] <= vec[monitor]) {
			begin++;
		}
		while (end > 0 && vec[end] > vec[monitor]) {
			end--;
		}
		int temp = 0;
		if (begin < end) {
			temp = vec[begin];
			vec[begin] = vec[end];
			vec[end] = vec[begin];
		}
		else {
			cout << "A" << endl;
			temp = vec[end];
			vec[end] = vec[monitor];
			vec[monitor] = temp;
		}
		for (int i = 0; i < size; i++) {
			cout << vec[i] << " ";
		}
		cout << endl;
	}
	/*
	if(size-end >= K){
		quickSort(vec, end+1, size-1,K,size);
	}else{
	*/
	quickSort(vec, monitor, end - 1, K, size);
	quickSort(vec, end + 1, size - 1, K, size);
	// }
};

int findKth(vector<int>& a, int n, int K) {

	quickSort(a, 0, n - 1, K, n);
	return a[n - K];
}
void test6() {
	vector<int> vec;
	for (int i = 10; i > 0; i--) {
		vec.push_back(i);
	}
	vector<int> a;
	a.assign(vec.begin(), vec.end());
	vec[0] = 11;
	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
	cout << endl;
	//int res = findKth(vec, 10, 3);
	//cout << res << endl;
}
void print_vector(vector<int>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i] << " ";
	}
	cout << endl;
}
vector<int> twoSum(vector<int>& numbers, int target) {
	vector<int> temp;
	temp.assign(numbers.begin(), numbers.end());
	sort(numbers.begin(), numbers.end());
	print_vector(temp);
	print_vector(numbers);
	int end = numbers.size() - 1;
	while (numbers[end] > target) {
		end--;
	}
	int begin = 0;
	while (end > begin&&numbers[begin] + numbers[end] != target) {
		while (end > begin&&numbers[begin] + numbers[end] > target) {
			end--;
		}
		while (end > begin&&numbers[begin] + numbers[end] < target) {
			begin++;
		}
	}
	vector<int> res;
	int flag = 0;
	cout << temp[28] << endl;
	cout << temp[45] << endl;
	cout << "begin:" << begin << endl;
	cout << "end:" << end << endl;
	cout << "numbers[begin]:" << numbers[begin] << endl;
	cout << "numbers[end]:" << numbers[end] << endl;
	bool begin_flag = false;
	bool end_flag = false;
	int a, b;
	for (int i = 0; i < temp.size(); i++) {
		if (flag == 0 && (temp[i] == numbers[begin] || temp[i] == numbers[end])) {
			a = i;
			flag++;
		}
		else if (flag == 1 && (temp[i] == numbers[begin] || temp[i] == numbers[end])) {
			b = i;
			flag++;
		}
		if (flag == 2)
			break;
		/*
		if(temp[i]==numbers[begin]&&begin_flag==false){
			begin = i;
			begin_flag = true;
		}else if(temp[i]==numbers[end]&&end_flag==false){
			end = i;
			end_flag = true;
		}
		if(begin_flag==true&&end_flag==true){
			break;
		}*/
	}
	res.push_back(a + 1);
	res.push_back(b + 1);
	return res;
}
void test7() {
	vector<int> vec;
	vec = { 230,863,916,585,981,404,316,785,88,12,70,435,384,778,887,755,740,337,86,92,325,422,815,650,920,
		125,277,336,221,847,168,23,677,61,400,136,874,363,394,199,863,997,794,587,124,321,212,957,764,173,314,
		422,927,783,930,282,306,506,44,926,691,568,68,730,933,737,531,180,414,751,28,546,60,371,493,370,527,387,
		43,541,13,457,328,227,652,365,430,803,59,858,538,427,583,368,375,173,809,896,370,789 };
	
	vector<int> res = twoSum(vec, 542);
	print_vector(res);
	cout << endl;
}

struct ListNode {
	int val;
	struct ListNode *next;
};
void print_link(ListNode* head) {
	while (head != nullptr) {
		cout << head->val << " ";
		head = head->next;
	}
	cout << endl;
}
ListNode* create() {
	ListNode* end = nullptr;
	ListNode* head = (ListNode*)malloc(sizeof(ListNode));
	head->val = -1;
	head->next = nullptr;
	end = head;
	for (int i = 0; i < 10; i++) {
		ListNode* node = (ListNode*)malloc(sizeof(ListNode));
		node->val = i;
		node->next = nullptr;
		end->next = node;
		end = node;
	}
	return head;
}
ListNode* reverse(ListNode* head, int k) {
	ListNode* res = head->next;
	ListNode* pre = head->next;
	ListNode* cur = pre->next;
	while (k > 1) {
		cout << "c" << endl;
		pre->next = cur->next;
		cur->next = head->next;
		head->next = cur;
		//pre = pre->next;
		cur = pre->next;
		k--;
	}
	return res;
};
inline bool is_empty1(ListNode* _head, int k) {
	while (_head->next != nullptr&&k > 0) {
		//cout << "B" << endl;
		_head = _head->next;
		k--;
	}
	return k == 0 ? true : false;
}
ListNode* reverseKGroup(ListNode* head, int k) {
	if (head == nullptr)
		return head;
	ListNode* _head = (ListNode*)malloc(sizeof(ListNode));
	_head->next = head;
	_head->val = -1;
	ListNode* temp_head = _head;
	while (temp_head != nullptr&&is_empty1(temp_head, k)) {
		cout << "A" << endl;
		cout << temp_head->val << endl;
		temp_head = reverse(temp_head, k);
		print_link(_head->next);
	}
	return _head->next;
}

void test8() {
	int i = 0;
	const int * ptr = &i;
	int* const pt = &i;
	ListNode* _head = create();
	print_link(_head->next);
	_head = reverseKGroup(_head->next, 3);
	print_link(_head);
}
string find_string(char* str1,int i, char* str2,int j) {
	string res;
	while (j>0&&i>0&&*str1 == *str2) {
		res += *str1;
		str1++;
		str2++;
		i--;
		j--;
	}
	return res;
};
string LCS(string str1, string str2) {
	string res;
	int len1 = str1.length();
	int len2 = str2.length();
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			string s = find_string((char*)str1[i],len1-i, (char*)str2[j],len2-j);
			if (s.length() > res.length()) {
				res = s;
			}
		}
	}
	return res;
}
void test9() {
	cout << 2 << endl;
	string res = LCS("1234G", "A123B");
	if (res == "123") {
		cout << 1 << endl;
	}
	else {
		cout << 3 << endl;
	}
}


ListNode* FindFirstCommonNode(ListNode* pHead1, ListNode* pHead2) {
	if (pHead1 == nullptr || pHead2 == nullptr)
		return nullptr;
	ListNode* temp1 = pHead1;
	ListNode* post1 = pHead1;
	ListNode* temp2 = pHead2;
	ListNode* post2 = pHead2;
	while (temp1 != nullptr || temp2 != nullptr) {
		if (temp1 != nullptr) {
			if (temp1->next != temp1) {
				post1 = temp1->next;
				temp1->next = temp1;
				temp1 = post1;
			}
			else {
				return temp1;
			}
		}
		if (temp2 != nullptr) {
			if (temp2->next != temp2) {
				post2 = temp2->next;
				temp2->next = temp2;
				temp2 = post2;
			}
			else {
				return temp2;
			}
		}
	}
	return nullptr;
}

void test10() {
	ListNode* end1 = nullptr;
	ListNode* end2 = nullptr;
	ListNode* head1 = (ListNode*)malloc(sizeof(ListNode));
	ListNode* head2 = (ListNode*)malloc(sizeof(ListNode));
	head1->val = -1;
	head1->next = nullptr;
	head2->val = -1;
	head2->next = nullptr;
	end1 = head1; 
	end2 = head2;
	for (int i = 0; i < 10000; i++) {
		ListNode* node = (ListNode*)malloc(sizeof(ListNode));
		node->val = i;
		node->next = nullptr;
		end1->next = node;
		end1 = node;
		if (i > 4) {
			ListNode* node1 = (ListNode*)malloc(sizeof(ListNode));
			node1->val = i + 10;
			node1->next = nullptr;
			end2->next = node1;
			end2 = node1;
		}
		
	}
	for (int i = 111; i < 115;  i++) {
		ListNode* node = (ListNode*)malloc(sizeof(ListNode));
		node->val = i;
		node->next = nullptr;
		end1->next = node;
		end2->next = node;
		end1 = node;
		end2 = node;
	}
	ListNode* n = FindFirstCommonNode(head1, head2);
	cout << n->val << endl;
}