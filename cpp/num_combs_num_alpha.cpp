/*
Given a number print the number of combinations you can derive from the number.
1=A, 2=B, 26=Z, 0=+.

For example: 1123 can be represented by 1,1,2,3 which would stand for AABC.
Another representation - 11,23 - JW
Another representation - 1,1,23 - AAW
Another representation - 11,2,3 - JBC

For number 1123, there will be 5 combinations.


*/

#include <iostream>
#include <vector>

using namespace std;

void getCombs(char *s, vector<int> res, int &count) {

  if (s[0] == '\0') {
    count++;
    cout << ".(" << count << ") : ";
    for (int i = 0; i < res.size(); i++)
      cout << res[i] << ",";
    cout << endl;
    return;
  } else if (s[1] == '\0') {
    count++;
    res.push_back(s[0] - '0');
    cout << ".(" << count << ") : ";
    for (int i = 0; i < res.size(); i++)
      cout << res[i] << ",,";
    cout << endl;
    return;
  }

  int i1 = s[0] - '0';
  int i2 = s[1] - '0';
  int i3 = i1 * 10 + i2;

  cout << "\t" << i1 << " / " << i3 << endl;
  res.push_back(i1);
  getCombs(s + 1, res, count);
  res.pop_back();

  if (i3 <= 26) {
    res.push_back(i3);
  } else {
    return;
  }

  getCombs(s + 2, res, count);
}

int numPermuts(int num) {
  int lastTwo;
  if (num < 10)
    return 1;

  lastTwo = num % 100;

  if (lastTwo <= 26) {
    return numPermuts(num / 100) + numPermuts(num / 10);
  } else {
    return numPermuts(num / 10);
  }
}

int main() {

  char s[5] = "1323";
  vector<int> res;
  s[4] = '\0';
  int count = 0;
  getCombs(s, res, count);

  cout << "numPermuts: " << numPermuts(1323) << endl;

  return 0;
}
