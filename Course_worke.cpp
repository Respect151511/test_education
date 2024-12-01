#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Student {
	int ID;
	int group;
	int stipend;
	int student_course;
	int marks[5];
	std::string name;

	// Конструктор по умолчанию
	Student() : ID(0), group(0), stipend(0), student_course(0), name("") {
		for (int i = 0; i < 5; i++) {
			marks[i] = 0;
		}
	}

	// Конструктор с параметрами
	Student(int ID, int group, int stipend, int student_course, int marksArray[5], std::string name)
		: ID(ID), group(group), stipend(stipend), student_course(student_course), name(name.substr(0, 50)) {
		for (int i = 0; i < 5; i++) {
			this->marks[i] = marksArray[i];
		}
	}
};

template <typename T>
struct Node {
	T data;
	Node* next = nullptr;
	Node* prev = nullptr;
};

template <typename T>
struct List {
	int size = 0;
	int currentIndex = 0;
	Node<T>* current = nullptr;
	Node<T>* head = nullptr;
	Node<T>* tail = nullptr;
	void toFirst() {
		if (!size) return;
		current = head;
		currentIndex = 0;
	}
	void toLast() {
		if (!size) return;
		current = tail;
		currentIndex = count() - 1;
	}
	void toNext() {
		if (!size) return;
		current = current->next;
		currentIndex++;
	}
	void toPrev() {
		if (!size) return;
		current = current->prev;
		currentIndex--;
	}
	void currentTo(int index) {
		if (!size) return;
		if ((currentIndex - index) > index) {
			toFirst();
		}
		if ((count() - 1 - index) < (index - currentIndex)) {
			toLast();
		}
		while (index > currentIndex) {
			toNext();
		}
		while (index < currentIndex) {
			toPrev();
		}
	}
	void removeCurrent() {
		if (!size) return;
		Node<T>* delNode = current;
		if ((delNode != head) and (delNode != tail)) {
			delNode->prev->next = delNode->next;
			delNode->next->prev = delNode->prev;
			current = current->next;
		}
		if (delNode == head) {
			head = delNode->next;
			if (delNode->next != nullptr) {
				delNode->next->prev = nullptr;
				current = current->next;
			}
		}
		if (delNode == tail) {
			tail = delNode->prev;
			if (delNode->prev != nullptr) {
				delNode->prev->next = nullptr;
				toPrev();
			}
		}
		delete delNode;
		size--;
		if (!size) current = nullptr;
	}
	void insertBeforeCurrent(T data) {
		if (!size) return;
		Node<T>* newNode = new Node<T>;
		size++;
		currentIndex++;
		newNode->data = data;
		newNode->next = current;
		newNode->prev = current->prev;
		if (current->prev != nullptr) current->prev->next = newNode;
		current->prev = newNode;
		if (current == head) { head = newNode; }
	}
	Node<T>* getCurrent(int index) {
		currentTo(index);
		return current;
	}
	void add(T data) {
		Node<T>* newNode = new Node<T>;
		newNode->data = data;
		if (!size) {
			currentIndex = 0;
			current = newNode;
			head = newNode;
			tail = newNode;
		}
		else {
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
		size++;
	}
	void insert(int index, T data) {
		if ((!size) or (index == size)) {
			add(data);
		}
		else {
			currentTo(index);
			insertBeforeCurrent(data);
		}
	}
	void removeAt(int index) {
		currentTo(index);
		removeCurrent();
	}
	T elementAt(int index) {
		currentTo(index);
		return current->data;
	}
	int count() {
		return size;
	}
	void clear() {
		for (int i = size; i > 0; i--) {
			removeCurrent();
		}
	}
};

Student addStudent(int& ID) {
	Student student;
	student.ID = ID++;
	cout << endl;
	cout << "Student name: ";
	cin.ignore();
	getline(cin, student.name);
	cout << "Student group: ";
	cin >> student.group;
	cout << "Student marks: ";
	for (int i = 0; i < 5; i++) {
		cin >> student.marks[i];
	}
	cout << "Student stipend: ";
	cin >> student.stipend;
	cout << "Student course: ";
	cin >> student.student_course;
	return student;
}
void coutStudent(Student student) {
	cout << "Student ID: " << student.ID << endl;
	cout << "Student name: " << student.name << endl;
	cout << "Student group: " << student.group << endl;
	cout << "Student marks: ";
	for (int i = 0; i < 5; i++) {
		cout << student.marks[i] << " ";
	}
	cout << endl;
	cout << "Student stipend: " << student.stipend << endl;
	cout << "Student course: " << student.student_course << endl;
}
void removeGroup(List<Student>& studentList, int group) {
	for (int i = 0; i < studentList.count(); i++) {
		if (studentList.elementAt(i).group == group) {
			studentList.removeCurrent();
			i--;
		}
	}
}
void removeStudent(List<Student>& studentList, int ID) {
	for (int i = 0; i < studentList.count(); i++) {
		if (studentList.elementAt(i).ID == ID) {
			studentList.removeCurrent();
			return;
		}
	}
}
void studentsSortedByAverage(List<Student> list) {
	if (list.count() == 0) return;

	List<Student> outputList;

	
	for (int i = 0; i < list.count(); i++) {
		outputList.add(list.elementAt(i));
	}

	
	for (int i = 0; i < outputList.count() - 1; i++) {
		for (int j = i + 1; j < outputList.count(); j++) {
			
			double avg_i = 0, avg_j = 0;
			for (int k = 0; k < 5; k++) {
				avg_i += outputList.elementAt(i).marks[k];
				avg_j += outputList.elementAt(j).marks[k];
			}
			avg_i /= 5;
			avg_j /= 5;

			
			if (avg_i > avg_j) {
				Student temp = outputList.elementAt(i);
				outputList.removeAt(i);
				outputList.insert(i, outputList.elementAt(j - 1));
				outputList.removeAt(j);
				outputList.insert(j, temp);
			}
			
			else if (avg_i == avg_j && outputList.elementAt(i).name > outputList.elementAt(j).name) {
				Student temp = outputList.elementAt(i);
				outputList.removeAt(i);
				outputList.insert(i, outputList.elementAt(j - 1));
				outputList.removeAt(j);
				outputList.insert(j, temp);
			}
		}
	}

	
	for (int i = 0; i < outputList.count(); i++) {
		cout << outputList.elementAt(i).name << "\t" << outputList.elementAt(i).group << "\t"
			<< "Average grade: " << (double)(outputList.elementAt(i).marks[0] + outputList.elementAt(i).marks[1]
				+ outputList.elementAt(i).marks[2] + outputList.elementAt(i).marks[3] + outputList.elementAt(i).marks[4]) / 5 << endl;
	}
}
Student getStudentByID(List<Student> studentList, int ID) {
	for (int i = 0; i < studentList.count(); i++) {
		if (studentList.elementAt(i).ID == ID) {
			return studentList.elementAt(i);
		}
	}
	Student student;
	return student;
}
int toint(string str) {
	int num = 0, k = 0;
	for (int i = str.size() - 1; i >= 0; i--, k++) {
		num += (str[i] - 48) * pow(10, k);
	}
	return num;
}
int* tointarr(string str) {
	int arr[5] = {}, k = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] != ' ') {
			arr[k] = str[i] - 48;
			k++;
		}
	}
	return arr;
}
void fileInput(List<Student>& list, int& ID) {
	Student student;
	ifstream input;
	input.open("input.txt");
	string str;
	int number = 0;
	int* arr;
	while (getline(input, str)) {
		number++;
		switch (number) {
		case 1:
			student.name = str;
			break;
		case 2:
			student.group = toint(str);
			break;
		case 3:
			arr = tointarr(str);
			for (int i = 0; i < 5; i++) {
				student.marks[i] = arr[i];
			}
			break;
		case 4:
			student.stipend = toint(str);
			break;
		case 5:
			student.student_course = toint(str);
			student.ID = ID++;
			list.add(student);
			number = 0;
			break;
		}
	}
	input.close();
}

void fileOutput(List<Student> list) {
	ofstream output;
	output.open("output.txt");
	for (int i = 0; i < list.count(); i++) {
		Student student = list.elementAt(i);
		output << student.name << endl;
		output << student.group << endl;
		for (int j = 0; j < 5; j++) { output << student.marks[j] << " "; }
		output << endl << student.stipend << endl;
		output << student.student_course << endl;
		output << student.ID << endl;
	}
	output.close();
}

void menu() {
	cout << " 0 Terminate program" << endl;
	cout << " 1 Add student" << endl;
	cout << " 2 Remove student by ID" << endl;
	cout << " 3 Edit student by ID" << endl;
	cout << " 4 Show student by ID" << endl;
	cout << " 5 Show students by parameter" << endl;
	cout << " 6 Show all students" << endl;
	cout << endl;
	cout << " 7 Add group of students" << endl;
	cout << " 8 Remove group of students" << endl;
	cout << " 9 Change group number" << endl;
	cout << " 10 Show group of students" << endl;
	cout << " 11 Show students average marks" << endl;
	cout << endl;
	cout << " 12 File input" << endl;
	cout << " 13 File output" << endl;
}
void submenu() {
	cout << " 0 Cancel" << endl;
	cout << " 1 Name" << endl;
	cout << " 2 Student course" << endl;
	cout << " 3 Group" << endl;
	cout << " 4 Marks" << endl;
	cout << " 5 Stipend" << endl;
	cout << endl;
}
void submenu2() {
	cout << " 0 Cancel" << endl;
	cout << " 1 Name" << endl;
	cout << " 2 Student course" << endl;
	cout << " 3 Stipend" << endl;
	cout << endl;
}
void main() {
	List<Student> studentList;
	int ID = 1;
	int choice = 0;
	int choice2 = 0;
	int input = 0;
	while (true) {
		system("cls");
		menu();
		cout << "Choice: ";
		cin >> choice;
		switch (choice)
		{
		case (0): {
			return;
		}
		case (1): {
			cout << "Enter student data";
			studentList.add(addStudent(ID));
			cout << "Student is added" << endl;
			break;
		}
		case (2): {
			if (studentList.count() == 0) break;
			cout << "Enter student ID: ";
			cin >> input;
			removeStudent(studentList, input);
			cout << "Student is removed" << endl;
			break;
		}
		case (3): {
			if (studentList.count() == 0) break;
			cout << "ID of the student you want to edit: ";
			cin >> input;
			cout << "What do you want to edit?" << endl;
			submenu();
			cout << "Choice: ";
			cin >> choice2;
			switch (choice2)
			{
			case (0): {
				break;
			}
			case (1): {
				Student student = getStudentByID(studentList, input);
				if (student.ID == 0) break;
				removeStudent(studentList, input);
				string input2 = "";
				cout << "Enter new data: ";
				cin >> input2;
				student.name = input2;
				studentList.add(student);
				break;
			}
			case (2): {
				Student student = getStudentByID(studentList, input);
				if (student.ID == 0) break;
				removeStudent(studentList, input);
				int input2 = 0;
				cout << "Enter new data: ";
				cin >> input2;
				student.student_course = input2;
				studentList.add(student);
				break;
			}
			case (3): {
				Student student = getStudentByID(studentList, input);
				if (student.ID == 0) break;
				removeStudent(studentList, input);
				int input2 = 0;
				cout << "Enter new data: ";
				cin >> input2;
				student.group = input2;
				studentList.add(student);
				break;
			}
			case (4): {
				Student student = getStudentByID(studentList, input);
				if (student.ID == 0) break;
				removeStudent(studentList, input);
				int input2 = 0;
				cout << "Enter new data: ";
				for (int i = 0; i < 5; i++) {
					cin >> input2;
					student.marks[i] = input2;
				}
				studentList.add(student);
				break;
			}
			case (5): {
				Student student = getStudentByID(studentList, input);
				if (student.ID == 0) break;
				removeStudent(studentList, input);
				int input2 = 0;
				cout << "Enter new data: ";
				cin >> input2;
				student.stipend = input2;
				studentList.add(student);
				break;
			}
			default: {
				cout << "Unknown command, try again" << endl;
				break;
			}
			}
			break;
		}
		case (4): {
			if (studentList.count() == 0) break;
			cout << "Enter student ID: ";
			cin >> input;
			coutStudent(getStudentByID(studentList, input));
			break;
		}
		case (5): {
			if (studentList.count() == 0) break;
			cout << "What parametr?" << endl;
			submenu2();
			cout << "Choice: ";
			cin >> choice2;
			switch (choice2)
			{
			case (0): {
				break;
			}
			case (1): {
				cout << "Name: ";
				string input2 = "";
				cin >> input2;
				for (int i = 0; i < studentList.count(); i++) {
					if (studentList.elementAt(i).name == input2) {
						coutStudent(studentList.elementAt(i));
						cout << endl;
					}
				}
				break;
			}
			case (2): {
				cout << "Student course: ";
				cin >> input;
				for (int i = 0; i < studentList.count(); i++) {
					if (studentList.elementAt(i).student_course == input) {
						coutStudent(studentList.elementAt(i));
						cout << endl;
					}
				}
				break;
			}
			case (3): {
				cout << "Stipent: ";
				cin >> input;
				for (int i = 0; i < studentList.count(); i++) {
					if (studentList.elementAt(i).stipend == input) {
						coutStudent(studentList.elementAt(i));
						cout << endl;
					}
				}
				break;
			}
			default: {
				cout << "Unknown command, try again" << endl;
				break;
			}
			}
			break;
		}
		case (6): {
			cout << "All students:" << endl;
			for (int i = 0; i < studentList.count(); i++) {
				coutStudent(studentList.elementAt(i));
			}
			break;
		}
		case (7): {
			cout << "Amount of students: ";
			cin >> input;
			cout << "Enter students data";
			for (int i = 0; i < input; i++) {
				studentList.add(addStudent(ID));
			}
			cout << "Students are added" << endl;
			break;
		}
		case (8): {
			if (studentList.count() == 0) break;
			cout << "Group to remove: ";
			cin >> input;
			removeGroup(studentList, input);
			cout << "Group is removed: " << endl;
			break;
		}
		case (9): {
			if (studentList.count() == 0) break;
			int input2;
			cout << "Group number to change: ";
			cin >> input;
			cout << "New group number: ";
			cin >> input2;
			for (int i = 0; i < studentList.count(); i++) {
				if (studentList.elementAt(i).group == input) {
					Student student = studentList.elementAt(i);
					removeStudent(studentList, student.ID);
					i--;
					student.group = input2;
					studentList.add(student);
				}
			}
			cout << "Group number changed" << endl;
			break;
		}
		case (10): {
			if (studentList.count() == 0) break;
			cout << "Group number to show: ";
			cin >> input;
			for (int i = 0; i < studentList.count(); i++) {
				if (studentList.elementAt(i).group == input) {
					coutStudent(studentList.elementAt(i));
				}
			}
			break;
		}
		case (11): {
			cout << "students sorted by Average:" << endl;
			studentsSortedByAverage(studentList);
			break;
		}
		case (12): {
			fileInput(studentList, ID);
			break;
		}
		case (13): {
			fileOutput(studentList);
			break;
		}
		default: {
			cout << "Unknown command, try again" << endl;
			break;
		}
		}
		system("pause");
	}
}