/*student.h*/

//
// iClicker Analysis program in modern C++.
//
// <<Michal Bochnak>>
// U. of Illinois, Chicago
// CS 341: Fall 2017
// Project 01
//

using namespace std;

// clas to keep info about the student
class Student
{
private:
	string clickerID;				// id of the clicker
	int questionsAnsweredTotal;		// total questions answered
	int questionsAnsweredCorrectly;	// total of answered correctly


public:
	//constructor
	Student() {
		clickerID = "XXXXXXXX";
		questionsAnsweredTotal = 0;
		questionsAnsweredCorrectly = 0;
	}

	//custom contructor
	Student(string id, int qAnswered, int qAnsweredCorrectly) {
		clickerID = id;
		questionsAnsweredTotal = qAnswered;
		questionsAnsweredCorrectly = qAnsweredCorrectly;
	}

	//getters
	string GetclickerID() {
		return clickerID;
	}

	int GetQuestionsAnsweredTotal() {
		return questionsAnsweredTotal;
	}

	int GetQuestionsAnsweredCorrectly() {
		return questionsAnsweredCorrectly;
	}

	//setters
	void SetclickerID(string id) {
		clickerID = id;
	}

	void SetquestionsAnswered(int number) {
		questionsAnsweredTotal = number;
	}

	void SetquestionsAnsweredCorrectly(int number) {
		questionsAnsweredCorrectly = number;
	}

	// methods

	// adds the specified number to the actual number 
	// of correct answers
	void UpdateQuestionsAnsweredCorrectly(int num) {
		questionsAnsweredCorrectly += num;
	}

	// adds the specified number to the actual number 
	// of total answers
	void UpdateQuestionsAnsweredTotal(int num) {
		questionsAnsweredTotal += num;
	}
};


// holds the info about the session
class SessionInfo {

private:
	std::vector<Student> students;
	string sessionID,		// session name
		date,				// date of session
		time;				// time of the session
	int numOfQuestions,		// number of questions asked
		numOfClickers;		// number of clickers used in session


public:
	// constructor
	SessionInfo() {
		sessionID = "undefined";
		date = "undefined";
		time = "undefined";
		numOfQuestions = 0;
		numOfClickers = 0;
	}

	// getters
	string GetSessionID() {
		return sessionID;
	}

	string GetDate() {
		return date;
	}

	string GetHour() {
		return time;
	}

	int GetNumOfQuestions() {
		return numOfQuestions;
	}

	int GetNumOfClickers() {
		return numOfClickers;
	}

	std::vector<Student> GetStudents() {

		return students;
	}

	double getPercentAnswered() {
		return ((double)totalAnswers() / GetTotalQuestions()) * 100;
	}

	double GetPercentAnsweredCorrectlyTotal() {
		return ((double)GetTotalCorrectAnswers() / GetTotalQuestions()) * 100;
	}

	int GetTotalCorrectAnswers() {
		int total = 0;

		for (auto s : students)
			total += s.GetQuestionsAnsweredCorrectly();

		return total;
	}

	int GetTotalQuestions() {
		return numOfQuestions * numOfClickers;
	}

	auto GetStudentsBeginning()
	{
		return students.begin();
	}

	auto GetStudentsEnding()
	{
		return students.end();
	}


	// setters
	void SetSessionID(string id) {
		sessionID = id;
	}

	void SetDate(string d) {
		date = d;
	}

	void SetTime(string h) {
		time = h;
	}

	int SetNumOfQuestions(int num) {
		numOfQuestions = num;
	}

	int SetNumOfClickers(int num) {
		numOfClickers = num;
	}

	// sets the id, date, and time
	void SetSessionInfo(string id, string d, string t) {
		sessionID = id;
		date = d;
		time = t;
	}

	// methods

	// display the stats about the session
	void showStats() {
		cout << "Date: " << date << endl;
		cout << "Time: " << time << endl;
		cout << "Name: \"" << sessionID << "\"" << endl;
		cout << "# questions: " << numOfQuestions << endl;
		cout << "# clickers: " << numOfClickers << endl;
	}

	// updates the actual number of questions
	// by specified parameter
	void updateNumOfQuestions(int num) {
		numOfQuestions += num;
	}

	// updates the actual number of questions
	// by specified parameter
	void updateNumOfClickers(int num) {
		numOfClickers += num;
	}

	// return number of total answers during the session
	int totalAnswers() {
		
		int total = 0;

		for (auto s : students)
			total += s.GetQuestionsAnsweredTotal();

		return total;
	}

	// adds student
	void AddStudent(Student s) {

		students.push_back(s);
	}
};