#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	double midterm, final;
	ofstream file;

	file.open("testfile");
	file << "Writing to this file \n";
	file.close();

	cout << "Enter midterm and final grades: ";
	cin >> midterm >> final;

	cout << "Enter assignment grades: " << endl;

	int x, sum = 0, count = 0;
	while (cin >> x) {
		++count;
		sum += x;
	}

	// Print 3 significant digits
	streamsize prec = cout.precision(3);

	cout << "Your course grade is " << 0.2 * midterm + 0.4 * final + 0.4 * sum / count << endl; 

	return 0; 
}