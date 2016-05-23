#include <iostream>

using namespace std;

class testC {
public:
	testC();
	~testC();
	testC(int);
	testC(const testC& source);
	testC& operator=(const testC& source);
	void printData(string str);
private:
	int prdata1;
};

// default
testC::testC()
{
	this->prdata1 = 0;
}

testC::~testC()
{

}

testC::testC(int a)
{
	this->prdata1 = a;
}


testC::testC(const testC& source)
{
	this->prdata1 = source.prdata1;
}

testC&
testC::operator=(const testC& source)
{
	this->prdata1 = source.prdata1;
	return *this;
}

void
testC::printData(string str)
{

	cout << str << ": " << this->prdata1 << endl;

}

int main()
{
	testC *pTestC1 = new testC(10);
	testC *pTestC2 = new testC();

	testC stTestC1 = *pTestC1;
	testC stTestC2(*pTestC2);

	pTestC1->printData("pTestC1");
	pTestC2->printData("pTestC2");
	stTestC1.printData("stTestC1");
	stTestC2.printData("stTestC2");

	return 0;
}