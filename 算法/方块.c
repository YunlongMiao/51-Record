#include <stdio.h>

int date(int n, int m1, int d1, int m, int m2, int d2) { //Ϊ���������ϼ��  �����岿������
	int i, j, k, t1, t2, q, y, sum = 0;
	int a[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; /*�����е�һ��Ԫ��ֱ�Ӹ�ֵΪ0�����·����Ӧ*/
	for (i = n; i <= m; i++) {      /*���ѭ�����ӿ�ʼ��ݿ�ʼ����ѭ��+1��ֱ������������*/
		if (i == n)
			t1 = m1;   //����ʼ�·ݸ�ֵ������t1
		else
			t1 = 1;   //�����������  �·ݿ�ʼ��ֵΪ1������ʼ��ݺ�Ŀ������� ��
		if (i == m)
			t2 = m2;   //��i������Ŀ���·�ʱ ��Ŀ���·ݵ�ֵ����t2
		else
			t2 = 12;
		for (j = t1; j <= t2; j++) { //t1,t2����������Ӧ���·ݲ�
			if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)//�ж��Ƿ�Ϊ���꣬��Ϊ���꽫���·����¸�ֵΪ29��
				a[2] = 29;
			else
				a[2] = 28;
			if (i == n && j == m1)
				q = d1;
			else
				q = 1;
			if (i == n && j == m2)
				y = d2;
			else
				y = a[j];
			for (k = q; k <= y; k++) {
				sum++;        //��ÿ���·���������
			}
		}
	}
	printf("Number of day:%d", sum - 1);
	return sum;   //����ֵ������������֮ǰ����������
}

int main() {
	int n, m, m1, m2, d1, d2;
	static int sum, day;
	printf("please input start data��every date use��-��divide��:");
	scanf_s("%d%d%d", &n, &m1, &d1);
	printf("please input now data��every date use��-��divide��:");
	scanf_s("%d%d%d", &m, &m2, &d2);
	date(n, m1, d1, m, m2, d2);
	day = sum;

	return 0;
}
