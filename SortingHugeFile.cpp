//============================================================================
// Name        : SortingHugeFile.cpp
// Author      : Adrian Harminto
// Comparing library sort (stable_sort) vs manually written sort
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include "Student.cpp"
using namespace std;

Student* readStudentsFromFile(string filename, int num) {
	ifstream studentsStream;
	studentsStream.open(filename.c_str());
	if (!studentsStream.is_open()) {
		cerr << "Couldn't open the file " << filename << endl;
		return NULL;
	}
	Student* students = new Student[num];
	string name, school, sid;
	int id;
	for (int i = 0; i < num; i++) {
		getline(studentsStream, name, ',');
		getline(studentsStream, sid, ',');
		getline(studentsStream, school);
		istringstream idConv(sid);
		idConv >> id;
		students[i] = Student(id, name, school);
	}
	studentsStream.close();
	return students;
}

void writeStudentsToFile(Student students[], int num, string filename) {
	ofstream output(filename.c_str());
	for (int i = 0; i < num; i++) {
		output << students[i].toString();
		if (i < num - 1) {
			output << endl;
		}
	}
	output.close();
}

bool compareBySchoolName(Student s1, Student s2) {
	return s1.getSchool()<s2.getSchool();
}

void sortByGroupById1(Student students[], int len) {
	stable_sort(students, students+len, compareBySchoolName);
}

int schoolToIndex(string school) {
	if (school == "UCB")  return 0;
	if (school == "UCD")  return 1;
	if (school == "UCI")  return 2;
	if (school == "UCLA") return 3;
	if (school == "UCM")  return 4;
	if (school == "UCSD") return 5;
	if (school == "UCSF") return 6;

	cerr << "Unknown school " << school << endl;
	return -1;
}

void sortByGroupById2(Student students[], int len) {
	int temp [7]={0}, //for counting schools
		temp2 [7]={0}; //for index of each schools
    Student *tempStudent = new Student[len];
    
	for (int i =0; i<len; i++) { //counting how many students per school inside temp
        temp[schoolToIndex(students[i].getSchool())]++;
    }
    
    for (int i=0; i<7; i++) { //putting initial position to assign/put student on each school
        for (int j=0; j<i;j++) { //this is k^2, but k is for schools. Regard to n (students), this is still a constant time
            temp2[i]+=temp[j];
        }
    }
    
    for (int i = 0; i<len; i++) { //now use that position recorded in temp2 (int) to fetch student, & copy it to tempStudent
        tempStudent[temp2[schoolToIndex(students[i].getSchool())]++] = students[i];
    }

	for (int i=0;i<len;i++) { //copy-pasting tempStudent to the original
		students[i]=tempStudent[i];
	}
    
    delete [] tempStudent; //free-ing object
}

int main() {
	const int LEN = 350000;

	time_t start, end;

	Student* uc1 = readStudentsFromFile("uc_students_sorted_by_id.txt", LEN);
	time(&start);
	sortByGroupById1(uc1, LEN);
	time(&end);
		cout << "Using library sort it took " << difftime(end, start) << " seconds."
				<< endl;

	Student* uc2 = readStudentsFromFile("uc_students_sorted_by_id.txt", LEN);
	time(&start);
	sortByGroupById2(uc2, LEN);
	time(&end);
			cout << "Using counting sort it took " << difftime(end, start) << " seconds."
					<< endl;

	writeStudentsToFile(uc1, LEN, "uc_by_school_by_id1.txt");
	writeStudentsToFile(uc2, LEN, "uc_by_school_by_id2.txt");
	
	return 0;
}