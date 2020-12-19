#pragma once
#include <stdio.h>
#include <iostream>
#include <queue>
using namespace std;
#pragma warning(disable:4996)

const int maxn = 200;
int m, n;
int ans;
int ans2[maxn][maxn];
struct Node
{
	int set2[maxn][maxn];
	int loc;
	int sum;
	Node() {};
	Node(int _set2[][maxn], int _loc, int _sum)
	{
		for (int i = 1; i < maxn; i++)
		{
			for (int j = 1; j < maxn; j++)
				set2[i][j] = _set2[i][j];
		}
		loc = _loc;
		sum = _sum;
	};
	friend bool operator <(Node a, Node b)
	{
		return a.sum > b.sum;
	}
};
void solve()
{
	priority_queue<Node> q;
	ans = (int)1e7;
	for (int i = 0; i < (1 << n); i++)
	{
		int j = i;
		int sum = 0;
		int vis2[maxn][maxn];
		memset(vis2, 0, sizeof(vis2));
		for (int s = 1; s <= n; s++)
		{
			if (j & 1 << (s - 1))
			{
				if (vis2[i][s] == 0)
					vis2[1][s] = 1;
				if (vis2[1][s - 1] == 0)
					vis2[1][s - 1] = 2;
				if (vis2[1][s + 1] == 0)
					vis2[1][s + 1] = 2;
				if (vis2[2][s] == 0)
					vis2[2][s] = 2;
				sum++;
			}
		}
		int t = 1;
		q.push(Node(vis2, t, sum));
	}
	while (!q.empty())
	{
		Node u = q.top();
		int loc = u.loc;
		q.pop();
		if (ans <= u.sum) continue;
		if (u.loc == m + 1)
		{
			bool flag = false;
			for (int i = 1; i <= m && !flag; i++)
			{
				for (int j = 1; j <= n && !flag; j++)
					if (u.set2[i][j] == 0)
						flag = true;
			}
			if (flag) continue;
			if (ans > u.sum)
			{
				ans = u.sum;
				for (int i = 1; i < maxn; i++)
				{
					for (int j = 1; j < maxn; j++)
						ans2[i][j] = u.set2[i][j];
				}
				for (int i = 1; i <= n; i++)
				{
					if (ans2[m + 1][i] == 1)
					{
						ans2[m][i] = 1;
						ans++;
					}
				}

			}
		}
		int sum = 0;
		int se2[maxn][maxn];
		memset(se2, 0, sizeof(se2));
		for (int i = 0; i <= m; i++)
		{
			for (int j = 0; j < maxn; j++)
				se2[i][j] = u.set2[i][j];
		}
		for (int i = 1; i <= n; i++)
		{
			if (se2[loc][i] == 0)
			{
				if (se2[loc][i] != 1)
					se2[loc][i] = 2;
				if (se2[loc + 1][i + 1] != 1)
					se2[loc + 1][i + 1] = 2;
				if (se2[loc + 1][i - 1] != 1)
					se2[loc + 1][i - 1] = 2;
				if (se2[loc + 1][i] != 1)
					se2[loc + 1][i] = 1;
				if (se2[loc + 2][i] != 1)
					se2[loc + 2][i] = 2;
				sum++;
			}
		}
		q.push(Node(se2, u.loc + 1, sum + u.sum));
	}
}
int protectTest()
{
	while (~scanf("%d%d", &m, &n))
	{
		if (m == 0 && n == 0)break;
		memset(ans2, 0, sizeof(ans2));
		solve();
		cout << ans << endl;
		bool flag = false;
		if (!flag)
		{
			for (int i = 1; i <= m; i++)
			{
				for (int j = 1; j <= n; j++)
					if (ans2[i][j] == 1)
					{
						printf("1 ");
					}
					else
						printf("0 ");
				cout << endl;
			}
			cout << endl;
		}
		else
			puts("-1");
	}
	return 0;
}
