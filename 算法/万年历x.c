#include <stdio.h>


int text(int year);
int text_n_1(int n, int s, int t);
int text_n_2(int n, int s, int t);
int day[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



int ZhengjiaRun[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}; //31 29 31 30 31 30 31 31 30 31 30 31
int ZhengjiaPing[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};


int main() {
	int n_1, y_1, r_1, n_2, y_2, r_2;
	int i, temp, j, day = 0, day1 = 0, day2 = 0, sum = 0;
	scanf("%d%d%d%d%d%d", &n_1, &y_1, &r_1, &n_2, &y_2, &r_2);
	for (i = n_1, day1 = 0; i < n_2; i++) {//计算从n_1到n_2的前一年共多少天
		if (text(i))
			day += 366;
		else
			day += 365;
	}
	day1 = text_n_1(n_1, y_1, r_1);//计算N_1在上面多算了多少天，减去
	day2 = text_n_2(n_2, y_2, r_2);//计算N_2过了多少天，加上
	sum = day - day1 + day2;
	printf("%d %d %d %d", day, day1, day2, sum);

}

int text_n_1(int n, int s, int t) { //多少天--
	int day;
	if (text(n))
		day = ZhengjiaRun[s - 1] + t;
	else
		day = ZhengjiaPing[s - 1] + t;
	return day;
}

int text_n_2(int n, int s, int t) { //过多少天++
	int day;
	if (text(n))
		day = ZhengjiaRun[s - 1] + t;
	else
		day = ZhengjiaPing[s - 1] + t;
	return day;
}

int text(int year) {//判断是否为闰年，是返回1，否返回0
	if ((year % 400 == 0) || (year % 4 == 0) && (year % 100 != 0))
		return 1;
	else
		return 0;
}







