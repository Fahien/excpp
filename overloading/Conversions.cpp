struct X
{
	X( int );

	X operator+( int );

	int i;
};

struct Y
{
	Y( X );

	Y operator+( X );

	operator int();

	int i;
};

extern X operator*( X, Y );

extern int f( X );

X x = 1;
Y y = x;
int i = 2;


/// Which conversions are used in each expression?
int main()
{
	// Built in
	i + 10;
	
	// More than one
	// int + int
	// Y + X
	y + 10;
	
	// More than one
	// int * int
	// X * Y
	y + 10 * y;

	// f(X)
	f(7);

	// No suitable conversion
	f(y);

	// int + int
	y + y;

	// int + int
	106 + y;
}
