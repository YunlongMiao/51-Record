#include <stdio.h>









int ZhengjiaRun[] = {31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}; //31 29 31 30 31 30 31 31 30 31 30 31
int ZhengjiaPing[] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

int dayp[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int dayr[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int NijiaPing[] = {0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 334, 365};

int NijiaRun[] = {0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 335, 366};
int text(int year);

int main() {
	int x1, y1, z1, x2, y2, z2;

	while (~scanf("%d%d%d%d%d%d", &x1, &y1, &z1, &x2, &y2, &z2)) {
		int i, temp, j, day1 = 0, day2 = 0, sum = 0;

		for (i = x1 + 1, j = 0; i < x2; i++)
			if (text(i) == 1)
				j++;

		if (text(x1)) { 	//前，月 日 闰年  剩多少天 10 8
			day1 = NijiaRun[12 - y1];
			day1 = day1 + dayr[y1] - z1;
		} else {
			day1 = NijiaPing[12 - y1];
			day1 = day1 + dayp[y1 - 1] - z1;
		}

		if (text(x2)) { //  过多少天1，2，3
			day2 = ZhengjiaRun[y2 - 2];
			//2002 2004
			day2 = day2 + z2;
		} else {
			day2 = ZhengjiaPing[y2 - 2];
			day2 = day2 + z2;
		}
		sum = day1 + day2 + (x2 - x1 - 1) * 365 + j;
		//	printf("%d %d %d %d", day1, day2, j, sum);
		printf("%d", sum);
	}

//	2002 5 31  2003 1 31
}

int text(int year) {
	if ((year % 400 == 0) || (year % 4 == 0) && (year % 100 != 0))
		return 1;
	else
		return 0;
}
