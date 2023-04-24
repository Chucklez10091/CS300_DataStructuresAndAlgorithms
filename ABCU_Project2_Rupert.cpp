//============================================================================
// Name        : CS300_Project2_Rupert.cpp
// Author      : Chuke Rupert
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <cctype>
using namespace std;

/*
 * Structure for holding Course information
 */
struct Course{
	string id;
	string title;
	vector<string> prerequisites;

	// default constructor
	Course():id(""){
	}

	// Constructor with no prerequisites
	Course(string id, string title):id(id), title(title){
	}

	// Constructor when all fields provided
	Course(string a, string b, vector<string> preReqs) : Course(a,b){
		this->prerequisites = preReqs;
	}

};

struct Node{
	Course course;
	Node* right;
	Node* left;

	// default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
    // default destructor
    ~Node(){
    	delete this->left;
    	delete this->right;
    }
};

class CourseTree{
	public:
	Node* root;

	CourseTree() {
		root = nullptr;
	}

	~CourseTree() {
		delete root;
	}

};

/*
 *  Add a new node to the tree
 */
void addNode(CourseTree& tree, Course toAdd){
	//cout << "Adding Node: " << toAdd.id << endl;
	// If the tree is empty, make toAdd the root node
	if (tree.root == nullptr) {
		//cout << "root is null" << endl;
		tree.root = new Node(toAdd);
	}
	else {
		Node* curr = tree.root;
		Node* parent;
		// iterate to find position for new node
		while(curr != nullptr){
			// parent follows curr to maintain coherence between tree and new node
			parent = curr;
			if (curr->course.id > toAdd.id) curr = curr->left;
			else if(curr->course.id < toAdd.id) curr = curr->right;
			// course is already on tree
			else{
				return;
			}
		}
		// make appropriate child a new node with course info
		if (parent->course.id > toAdd.id) parent->left = new Node(toAdd);
		else{
			parent->right = new Node(toAdd);
		}
	}
	return;
}

/*
 * Search for one specific course by id number
 */
Course treeSearch(CourseTree& tree, string idNum){
	Node* curr = tree.root;
	while (curr != nullptr){
		if (curr->course.id == idNum) return curr->course;
		if (curr->course.id > idNum) curr = curr->left;
		else curr = curr->right;
	}
	// Course not found
	cout << "Course not found!" << endl;
	return Course();
}

// Traverse a Tree or subtree in order from node and print course id and title
void inOrderList(Node* node){
	if(node == nullptr) return;
	inOrderList(node->left);
	std::cout << node->course.id << ", " << node->course.title << endl;
	inOrderList(node->right);
}

// Print all information for one course
void printCourseInfo(Course toPrint){
	// if search returns an empty course
	if (toPrint.id == "") {
		return;
	}
	std::cout << toPrint.id << ", " << toPrint.title << endl;
	std::cout << "Prerequisites: ";
	// If no prereqs listed
	if (toPrint.prerequisites.size() == 0){
		std::cout << "None" << endl;
	}
	// else course has prereqs to print
	else{
		// for all courses in list
		for (string id: toPrint.prerequisites){
			cout << id;
			// if last item found, go to next line
			if (id == toPrint.prerequisites.at(toPrint.prerequisites.size() - 1)){
				cout << endl;
			}
			// else prep next element
			else cout << ", ";
		}
	}
}

// Method for displaying menu to user
void printMenu(){
	cout << "1. Load Data Structure." << endl;
	cout << "2. Print Course List." << endl;
	cout << "3. Print Course." << endl;
	cout << "4. Exit" << endl << endl;
}

int getUserInput(){
	cout << "What would you like to do? ";
	int val;
	cin >> val;
	return val;
}

void getUserCourse(CourseTree& tree){
	string courseID;
	Course c;
	while (true){
		cout << "What course do you want to know about? ";
		cin >> courseID;
		 std::transform(courseID.begin(), courseID.end(), courseID.begin(), std::ptr_fun<int, int>(std::toupper));
		c = treeSearch(tree, courseID);
		if (c.id != "") break;
	}
	printCourseInfo(c);
}

// Load data into tree
void loadCourses(string fileName, CourseTree& tree){
	// Initialize variables
	std::ifstream inFS;
	vector<string> lineVals;
	string sub;
	string lineIn;
	vector<string> preq;

	inFS.open(fileName);
	if (inFS.is_open()) cout << inFS.rdbuf() << endl;
	inFS.close();
	inFS.open(fileName);
	while (std::getline(inFS, lineIn)){
		stringstream ss(lineIn);
		while (ss.good()){
			getline(ss, sub, ',');
			lineVals.push_back(sub);
		}
		if (lineVals.size() > 2) preq = {lineVals.begin()+ 2, lineVals.end()};
		addNode(tree, Course(lineVals.at(0), lineVals.at(1), preq));
		//inOrderList(tree.root);
		lineVals.clear();
		preq.clear();
		ss.clear();
	}

	return;
}

int main() {
	string fileName = "U:\\ABCU_Project2_Rupert\\src\\ABCU_Advising_Program_Input.txt";
	int choice;
	CourseTree ABCU_Tree = CourseTree();
	cout << "Welcome to the course planner." << endl << endl;
	while (true){
		printMenu();
		choice = getUserInput();
		switch (choice){
		case 1:
			if (ABCU_Tree.root == nullptr) loadCourses(fileName, ABCU_Tree);
			cout << ABCU_Tree.root->course.id << endl;
			break;
		case 2:
			if (ABCU_Tree.root == nullptr) cout << "Data structure not yet loaded!" << endl;
			else{
				cout << "Here is a sample schedule: \n" << endl;
				Node* x = ABCU_Tree.root;
				inOrderList(x);
				cout << endl;
			}
			break;
		case 3:
			if (ABCU_Tree.root == nullptr) cout << "Data structure not yet loaded!" << endl;
			else {
				getUserCourse(ABCU_Tree);
			}
			break;
		case 4:
			cout << "Thank you for using the course planner!" << endl;
			return 0;
			break;
		default:
			cout << choice << " is not a valid option." << endl << endl;
			choice = 1;
		}
	}
}
