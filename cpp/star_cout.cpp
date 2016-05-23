#include <iostream>
#include <string>
using namespace std;

string get_output(int len)
{
	string s(len, '*');
	return s;
}

int main()
{
	for (int i = 1; i <= 5 ; i++) {
		string s = get_output(i);
		cout << s << endl;
	}

	string str("There are two needles in this haystack with needles");
	string str2("needle");
	size_t found;

	found = str.find(str2);
	if (found != string::npos) 
		cout << "Found " << str2 << " at " << int(found) << endl;

	found = str.find(str2, found+1);
	if (found != string::npos) {
			cout << "Found second " << str2 << " at " << found << endl;
	}

	return 0;
}