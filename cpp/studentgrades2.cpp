#include <iostream>
#include <iomanip> // for setw
#include <algorithm> // for sort and max
#include <vector>
#include <stdexcept>
#include <fstream>
using namespace std;

struct student_info {
	string name;
	double midterm;
	double final;
	std::vector<double> homework;
};

istream& read_homework(istream& in, vector<double>& homework)
{
	if (in) {
		
		double x; 
		homework.clear();
	
		while (cin >> x) {
			homework.push_back(x);
		}

		if (homework.size() == 0) {
			throw domain_error("Need at least 1 hw");
		}

		in.clear(); 
	}

	return in;
}

double compute_median(vector<double>& v)
{
	if(v.size() == 0) {
		throw domain_error("cant compute median of empty vector");
	}

	vector<double> homework(v);
	
	sort(homework.begin(), homework.end());
	int mid = homework.size() / 2;
	double median;

	if (homework.size() % 2 == 0) {
		median = (homework[mid-1] + homework[mid]) / 2;
	} else {
		median = homework[mid];
	}

	return median;
}

istream& read(istream& in, student_info& s)
{
	cin >> s.name >> s.midterm >> s.final;
	return read_homework(in, s.homework);
}

// Sort names lexicographically
bool compare_names(student_info& s1, student_info& s2)
{
	return s1.name < s2.name;
}

double grade(double midterm, double final, vector<double>& homework)
{
	try {
		return (0.2 * midterm + 0.4 * final + compute_median(homework));
	} catch (domain_error& e) {
		throw domain_error("Need atleast 1 hw");
	}

	return 0;
}

double grade(student_info& s)
{
	return grade(s.midterm, s.final, s.homework);
}

bool failing_grade(student_info& s)
{
    return grade(s);
}

vector<student_info> extract_fails(vector<student_info>& students)
{
    int i = 0;
    vector<student_info> fail;

    while (i != students.size()) {
        if (failing_grade(students[i])) {
            fail.push_back(students[i]);
            students.erase(students.begin() + i);
        } else {
            ++i;
        }
    }
    return fail;
}

int main()
{
	double midterm, final;
	ofstream file;

	file.open("testfile");
	file << "Writing to this file \n";
	file.close();

	student_info student;
	vector<student_info> students;
	string::size_type maxlen = 0;
	while(read(cin, student)) {
		maxlen = max(maxlen, student.name.size());
		students.push_back(student);
	}

	// Sort students based on their names so as to alphabetize the list
	sort(students.begin(), students.end(), compare_names);
	
	for (int i = 0; i < students.size(); ++i) {
		cout << setw(maxlen + 1) << students[i].name;
		try {
			double course_grade = grade(students[i]);
			streamsize old_prec = cout.precision(3);
			cout << "Your course grade is " << course_grade << endl; 
			cout.precision(old_prec);
		} catch	(domain_error& e) {
			cout << e.what();
		}
	}

	return 0; 
}
