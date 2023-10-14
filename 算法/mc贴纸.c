#include <stdio.h>

int main() {
	int n, i, j, h, x = 0, min = 9999;
	scanf("%d", &n);
	for (i = 1; i <= n  ; i++)
		for (j = i; j <= n  ; j++) {
			if (i * j > n)
				continue;
			if (n % (i * j) == 0) {
				h = n / (i * j);
				x = i * j * 2 + i * h * 2 + 2 * j * h;
			}
			if (x < min)
				min = x;
		}
	printf("%d", min);

}
