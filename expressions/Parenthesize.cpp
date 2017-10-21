#include <iostream>

int f(int a, int b)
{
	return a + b;
}

/// Fully parenthesize the following expressions
int main()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int x = 5;
	int i = 6;

	a = b + c * d << 2 & 8;
	a = (((b + (c * d)) << 2) & 8);

	a & 077 != 3;
	a & (077 != 3);

	a == b || a == c && c < 5;
	((a == b) || ((a == c) && (c < 5)));

	c = x != 0;
	c = (x != 0);

	0 <= i < 7;
	((0 <= i) < 7);

	a = -1 + +b-- -5;
	a = (((-1) + (+(b--))) - 5);

	a = b == c++;
	a = (b == (c++));

	a = b = c = 0;
	a = (b = (c = 0));
	
	{
		int a[5][3] = {0};
		int *b = a[0];
		int *d = a[1];
		a[4][2] *= *b ? c : *d * 2;
		a[4][2] *= ((*b) ? c : ((*d) * 2));
	}

	a - b, c = d;
	(a - b), (c = d);
}