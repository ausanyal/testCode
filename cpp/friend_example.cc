#include <iostream>

using namespace std;

class Square;

class Rectangle {
	int w, h;
public:
	Rectangle() {};
	Rectangle(int x, int y) : w(x), h(y) {};
	int area() {return (w * h);};
	void convert(const Square& s);
	friend Rectangle duplicate(const Rectangle& param);
};

class Square {
friend class Rectangle;
private:
	int side;
public:
	Square(int a) : side(a) {};
};

void
Rectangle::convert(const Square& s)
{
	this->w = s.side;
	this->h = s.side;
}

Rectangle duplicate(const Rectangle& param)
{
	Rectangle res;
	res.w = param.w;
	res.h = param.h;

	return res;
}

int main()
{
	Rectangle a(5, 5);
	Rectangle b = duplicate(a);

	cout << "a: " << a.area() << endl;
	cout << "b: " << b.area() << endl;


	Square s(10);
	b.convert(s);
	
	cout << "b: " << b.area() << endl;	

	return 0;
}