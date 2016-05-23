#include <iostream>
using namespace std;

string reverse_str(string str)
{
	/*
	for (int i = 0, j = str.size()-1 ; i < str.size()/2 ; i++, j--) {
		swap(str[i], str[j]);
		if (str[i] != str[j]) {
			plndrm = 0;
		}
	}
	*/

	for (int i = 0 ; i < str.size()/2 ; i++) {
		swap(str[i], str[str.size()-i-1]);
	}
	
	return str;
}

int main()
{
	int plndrm = 0;
	string str, rev_str;
	cout << "Enter Str:" << endl;
	cin >> str;
	
	rev_str = reverse_str(str);

	if (str == rev_str) {
		plndrm = 1;
	}

	cout << "str: " << str << " reversed: " << rev_str << " is plndrm: " << plndrm << endl;

	return 0;
}