#include <iostream>

using namespace std;

class testClass {
public:
  testClass(int x, int y);
  ~testClass();
  void print_vars();
private:
  int x_, y_;
  int t_x, t_y;
};

testClass::testClass(int x, int y)
  : x_(x),y_(y) {
  this->t_x = x_;
  this->t_y = y_;
}

testClass::~testClass()
{

}

void
testClass::print_vars()
{
  cout << "x: " << this->t_x << " y: " << this->t_y << endl;
}

int main(int argc, char const *argv[]) {

  testClass x(10, 20);
  x.print_vars();

  return 0;
}
