/*
given a string with only paranthesis - find out if it is balanced or not
eg {}[]()
followup : scale your solution and specify the right data structure to use if
you have a lot of such bracket types

*/

#include <iostream>
#include <map>
#include <stack>

using namespace std;

typedef map<char, char>::iterator it;
int checkBalanced(char *str) {

  map<char, char> parenthesisType;
  stack<char> mystack;

  parenthesisType.insert(make_pair('(', ')'));
  parenthesisType.insert(make_pair('{', '}'));
  parenthesisType.insert(make_pair('[', ']'));

  it x;

  for (int i = 0; i < strlen(str); i++) {

    x = parenthesisType.find(str[i]);
    if (x != parenthesisType.end()) {
      mystack.push(str[i]);
      cout << "Pushing " << str[i] << " top " << mystack.top() << endl;
      continue;
    }

    if (mystack.size()) {
      x = parenthesisType.find(mystack.top());
      if (x != parenthesisType.end() && (*x).second == str[i]) {
        cout << "Found matching " << (*x).second << " vs " << mystack.top()
             << endl;
        mystack.pop();
      } else
        cout << "(1) Error\n";
    } else
      cout << "(2) Error \n";
  }

  if (mystack.size()) {
    cout << "Not balanced\n";
    for (int i = 0; i < mystack.size(); i++) {
      cout << mystack.top() << " ";
      mystack.pop();
    }
    cout << endl;
  }

  return 0;
}

int main() {
  char str[] = {'{', '}', '[', ']', ')'};
  checkBalanced(str);
  return 0;
}
