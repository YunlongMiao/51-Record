/*
用户输入一个正整数 N(3 \le N \le 9)N(3≤N≤9)。从 00 到 NN 之间的所有正整数（包含 00 和 NN）中选择三个，组成一个三位数（00 不能作为百位数），且这个三位数为奇数，请计算出共有多少种满足条件的三位数组合。（注意：组成的每个三位数各个位上的数字不能重复）

输入格式
输入一个正整数 N(3 \le N \le 9)N(3≤N≤9)。

输出格式
输出满足条件的三位数组合的个数。
*/

#include <stdio.h>

int main() {
	int n;
	int i, j, t, s;
	scanf("%d", &n); //0 1 2 3
	for (i = 1, s = 0; i <= n; i++)
		for (j = 0; j <= n; j++) {
			if (i != j) {
				for (t = 1; t <= n; t += 2)
					if (t != i && t != j)
						s++;
			}
		}
	printf("%d", s);
}

