#include"Stack.h"
#pragma warning(disable : 4996)
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctype.h>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
//#include"Queue.h"


class Solution
{
public:
	int largestRectangleArea(vector<int>& heights)
	{
		heights.push_back(0);
		const int size_h = heights.size();//Öù×ÓÊý	
		cout << "size_h = " << size_h << endl;
		Stack<int> stk;
		int i = 0, max_a = 0;
		while (i < size_h) {
			if (stk.empty() || heights[i] >= heights[stk.top()]) {
				stk.push(i++);
			}
			else {
				int h = stk.top();
				stk.pop();
				max_a = max(max_a, heights[h] * (stk.empty() ? i : i - stk.top() - 1));
			}
		}

		return max_a;
	}
};

int main()
{
	//2
	srand(time(nullptr));
	Solution a;
	for (int t = 1; t <= 10; ++t) {
		int n = rand() % 100000 + 1;
		vector<int> heights(n);
		for (int i = 0; i < n; ++i) {
			heights[i] = rand() % 10001;
		}
		cout << t << ": ";
		cout << a.largestRectangleArea(heights) << endl;
	}

}
