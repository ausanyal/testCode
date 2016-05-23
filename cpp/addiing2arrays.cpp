/*
There are 2 arrays of integers.You have to add the those integers and keep it in
3rd array.there is one condition, if the sum is a 2 digit number, split that
number into single digiit and other condition is if any of the array integer is
left then print that number
I/P:
int[] a = {1,2,3,4,5,6}
int[] b = {2,3,4,5,6,7,8}
o/p:
{3,5,7,9,1,1,1,3,8}
*/

#include <iostream>

using namespace std;

int main() {
  int a[] = {8, 7, 6, 4, 5, 6};
  int b[] = {2, 3, 4, 5, 6, 7, 8};

  int l1 = sizeof(a) / sizeof(a[0]);
  int l2 = sizeof(b) / sizeof(b[0]);
  int l = l1 + l2;
  cout << l << endl;
  int mx = max(l1, l2);
  int mn = min(l1, l2);
  int *c = new int[mx]();
  int *newc;
  int lastSz = mx;

  int r = 0, r1 = 0, r2 = 0;
  int j = 0, i;
  for (i = 0; i < mn; i++) {
    r = a[i] + b[i];
    cout << "i: " << i << " j: " << j << " Sum: " << r << endl;

    if (j == lastSz) {
      cout << "Increasing size from " << lastSz << " to " << lastSz + 2 << endl;
      newc = new int[lastSz + 2];
      copy(c, c + lastSz, newc);
      lastSz += 2;
      delete[] c;
      c = newc;
    }

    if (r > 9) {
      r1 = r / 10;
      r2 = r % 10;
      c[j++] = r1;
      c[j++] = r2;
    } else {
      c[j++] = r;
    }

    for (int k = 0; k < lastSz; k++) {
      cout << c[k] << " ";
    }
    cout << endl;
  }

  cout << "i: " << i << " l1: " << l1 << " l2: " << l2 << endl;
  int d1, d2;
  if (i < l1) {
    d1 = l1 - i;
    d2 = lastSz - j;
    if (d2 < d1) {
      newc = new int[lastSz + (d2 - d1 + 1)];
      copy(c, c + lastSz, newc);
      lastSz += 2;
      delete[] c;
      c = newc;
    }
    for (int k = i; k < l1; k++) {
      c[j++] = a[k];
    }
  } else if (i < l2) {
    d1 = l2 - i;
    d2 = lastSz - j;
    if (d2 < d1) {
      newc = new int[lastSz + (d2 - d1 + 1)];
      copy(c, c + lastSz, newc);
      lastSz += 2;
      delete[] c;
      c = newc;
    }
    for (int k = i; k < l2; k++) {
      c[j++] = b[k];
    }
  }

  for (int k = 0; k < j; k++) {
    cout << c[k] << " ";
  }
  cout << endl;

  return 0;
}
