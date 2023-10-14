#include <stdio.h>

int main() {
	int N;
	int sum; //0 1 2 3 4
	scanf("%d", &N);
	sum = N * N * (N - 1);
	if (N % 2 == 0)
		sum = sum / 2;
	printf("%d", sum);
}