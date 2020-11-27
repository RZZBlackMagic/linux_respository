#pragma once
// Perm.c : �������̨Ӧ�ó������ڵ㡣
//


#include<conio.h>
#include<iostream>
#define MAXM 10

void Perm(int *, int, int, int, int, int set[MAXM][MAXM], int nlist[MAXM]);

using namespace std;

int qlist[MAXM];    //���ľ�鳤��
int loss;           //���ÿ���и����ʧ
int n, len, mn = 0;      //���ľ������������ȣ�������Ҫ��ľ�ϵ�����

int test()
{
	int *list;

	int set[MAXM][MAXM];
	int nlist[MAXM];

	memset(qlist, 0, sizeof(qlist));
	cin >> len;
	cin >> loss;

	n = 0;
	//ÿ����������֮����-1��Ϊ������������ֻ����һ������,��Ҫ����������ݿ����ʵ��޸�
	while (cin >> qlist[n] && qlist[n] != -1)
		n++;

	mn = n;
	list = new int[n];
	for (int i = 0; i < n; i++) {
		list[i] = i;
	}

	memset(set, 0, sizeof(set));
	memset(nlist, 0, sizeof(nlist));
	Perm(list, 0, 0, n - 1, 1, set, nlist);

	return 0;
}

/************************************************************************************************
  Perm�����������壺
  list-->ԭʼ���У����ÿһ��ľ��ı��
  level->�������ڼ���
  k-->list����������ʼλ��
  m-->list�����������λ��
  count->�ڼ���ľ�ϣ�ľ�ϵ�����
  set->��ά���󣬴��ÿһ��ľ���зŵ��ǵڼ���ľ�飬set[i][j]=1��ʾ��i��ľ�Ͽ��Ծ�ɵ�j��ľ��
  nlist->��ǰ���д�ŵ�list�еĵڼ���ľ�飬��������п�����ɵ�ǰľ�ϵ�ľ��
***************************************************************************************************/

void Perm(int *list, int level, int k, int m, int count, int set[MAXM][MAXM], int nlist[MAXM])
{
	int i;
	int nset[MAXM][MAXM];
	int mlist[MAXM];
	int klist[MAXM];

	if (m == 0) {
		for (int l = k; l <= m; l++) {
			set[count - 1][list[l]] = 1;
		}
		//�ж�ÿһ����ľ�鳤���ܺ�+ÿ�ηϵ���ľ���Ƿ񳬹�һ��ľ�ϵĳ���
		int flag = 0;
		for (int l = 0; l < count; l++) {
			int sum = 0;
			for (int h = 0; h < n; h++) {
				if (set[l][h] == 1) {
					sum += qlist[h] + loss;
				}
			}
			if (sum > len + loss) {
				flag = 1;
				break;
			}
		}
		//������еĶ����㣬��ǰľ�ϵĿ�����һ��ѡ��
		if (flag == 0)
			mn = mn > count ? count : mn;
		return;
	}

	//����Ѿ�ȡ�����е�ľ�飬�򷵻���һ��ѡȡ����ѡ�ķ���
	if (k > m) {

		return;
	}
	else {
		//����ÿһ��ľ�϶���k-m��ѡȡ����������ȱ�����ÿ��ȡ��һ��ľ�飬ȷ�����ظ�ȡ
		for (i = k; i <= m; i++) {

			//��ȡ��һ���ڸ�����ѡȡ��ľ��
			for (int l = 0; l < level; l++)
				klist[l] = nlist[l];
			//����ȡ��һ���ľ��
			klist[level] = list[i];

			//��ȡ��ʣ���ľ��
			int left = 0;
			for (int l = 0; l <= m; l++) {
				int s;
				for (s = 0; s <= level; s++) {
					if (list[l] == klist[s])
						break;
				}
				if (s > level) {
					mlist[left++] = list[l];
				}
			}

			//���õ�ǰľ����ѡȡ������ľ�飬nset[l][h]=1��ʾ��l��ľ�Ͽ��ܾ�ɵ�h��ľ��
			for (int l = 0; l < MAXM; l++) {
				for (int h = 0; h < MAXM; h++) {
					nset[l][h] = set[l][h];
				}
			}

			for (int l = 0; l <= level; l++) {
				nset[count - 1][klist[l]] = 1;
			}

			//������е�ľ�����ȡ�꣬����һ��ѡ���������������ľ�ϵ���������������С��ľ����
			left--;
			if (left < 0) {
				int flag = 0;
				for (int l = 0; l < count; l++) {
					int sum = 0;
					for (int h = 0; h < n; h++) {
						if (nset[l][h] == 1) {
							sum += qlist[h] + loss;
						}
					}
					if (sum > len + loss) {
						flag = 1;
						break;
					}
				}
				if (flag == 0)
					mn = mn > count ? count : mn;
			}

			//��ʣ��ľ����Ϊ��һ��ľ�ϵ�����ѡȡ����������
			Perm(mlist, 0, 0, left, count + 1, nset, klist);

			//ȡ��ǰľ������Ӧ����һ��ľ���ȡ����ÿ��ȡ��һ��ľ�飬ȷ�����ظ�ȡ
			Perm(list, level + 1, i + 1, m, count, set, klist);
		}
	}
	return;
}






