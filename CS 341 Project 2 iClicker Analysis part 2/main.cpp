/*main.cpp*/

//
// iClicker Analysis program in modern C++.
//
// <<Michal Bochnak>>
// U. of Illinois, Chicago
// CS 341: Fall 2017
// Project 01
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

#include "student.h"

using namespace std;

// function prototypes
bool ReadTheData(std::string filename,
	SessionInfo &sessionInfo);
string findSessionID(string line);
string getDateFromFileName(string filename);
string getTimeFromFileName(string filename);
std::vector<char> getCorrectAnswers(string line);
void updateStudentInfo(string line, std::vector<char> correctAnswer,
	SessionInfo &si);
void findAndUpdatedStudent(char studentAnswer, std::vector<char> correctAnswers,
	string clickerID, SessionInfo &si);
void showStatistics(std::vector<Student> &s, SessionInfo &si);
void showStudentsWhoAnsweredTheQuestions(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredAllQuestions(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredAtLeastHalfQuestions(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredAtLeastOneQuestions(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredZeroQuestions(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredAllQuestionsCorrectly(std::vector<Student> s, SessionInfo si);
void showStudentsWhoAnsweredTheQuestionsCorrectly(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredAtLeastHalfQuestionsCorrectly(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredAtLeastOneQuestionsCorrectly(std::vector<Student> s, SessionInfo si);
int numOfStudentsAnsweredZeroQuestionsCorrectly(std::vector<Student> s, SessionInfo si);
bool answeredCorrectly(char studentAns, std::vector<char> correctAnws);
void listStudentsWithLessThanHalfCorrect(std::vector<Student> s, SessionInfo si);
void listStudentsWithZeroCorrect(std::vector<Student> s, SessionInfo si);
void processFiles(std::vector<SessionInfo> &si);
void showClassAnalysis(std::vector<SessionInfo> SessionInfo);
int totalQuestions(const std::vector<SessionInfo> si);
void showStudentAnalysis(std::vector<SessionInfo> sessionsInfo);


//
// FileExists:
//
// Returns true if the file exists, false if not.
//
bool FileExists(string filename)
{
	ifstream file(filename);

	return file.good();
}


int main()
{
	string filename;

	cout << "**Starting**" << endl;

	//
	// Open file and analyze the clicker data:
	//

	// create the vector with sessions
	std::vector<SessionInfo> sessionsInfo;

	// read data from the all files
	processFiles(sessionsInfo);

	// show stats
	showClassAnalysis(sessionsInfo);
	showStudentAnalysis(sessionsInfo);

	cout << endl << endl << "**END**" << endl;

	return 0;
}


// displays info about class analysis
void showClassAnalysis(std::vector<SessionInfo> si) {

	cout << "**Class Analysis**" << endl;
	cout << ">>Total sessions:  " << si.size() << endl;
	cout << ">>Total questions: " << totalQuestions(si) << endl;
	
	cout << ">>Answered:" << endl;
	for (auto s : si)
		cout << "  \"" << s.GetSessionID() << "\": " << s.getPercentAnswered() <<
				"% (" << s.GetNumOfQuestions() << " questions, " << s.GetNumOfClickers() <<
				" clickers, " << s.totalAnswers() << " answers)" << endl;

	cout << ">>Correctly:" << endl;
	for (auto s : si)
		cout << "  \"" << s.GetSessionID() << "\": " << s.GetPercentAnsweredCorrectlyTotal() <<
		"% (" << s.GetNumOfQuestions() << " questions, " << s.GetNumOfClickers() <<
		" clickers, " << s.GetTotalCorrectAnswers() << " correct answers)" << endl;	

	cout << endl;
}

// display student analysis
void showStudentAnalysis(std::vector<SessionInfo> sessionsInfo) {

	string clickerID;
	int count = 0;
	cout << "**Student Analysis**" << endl;
	cout << ">> Enter a clicker id (# to quit): ";
	cin >> clickerID;

	while (clickerID != "#") {

		count = 0;
		for (auto s : sessionsInfo) {
			auto iter = find_if(s.GetStudentsBeginning(), s.GetStudentsEnding(),
				[=](Student s) {
				return clickerID == s.GetclickerID();
			});

			if (iter != s.GetStudentsEnding()) {
				count++;	// found, increment count
				cout << "  \"" << s.GetSessionID() << "\": " <<
					iter->GetQuestionsAnsweredTotal() << " out of " <<
					s.GetNumOfQuestions() << " answered, " <<
					((double)iter->GetQuestionsAnsweredCorrectly() /
					s.GetNumOfQuestions()) * 100 << "% correctly" << endl;
			}
		}

		// zero found
		if (count == 0)
			cout << endl << "** not found..." << endl;

		cout << endl << "Enter a clicker id (# to quit): ";
		cin >> clickerID;
	}
}


// counts the total number of questions
int totalQuestions(const std::vector<SessionInfo> si) {

	int total = 0;

	for (auto s : si)
		total += s.GetNumOfQuestions();

	return total;
}


// read data form all the files
void processFiles(std::vector<SessionInfo> &si) {

	// open the file with filenames
	ifstream file("files.txt");
	string filename;

	while (getline(file, filename)) {

		cout << ">>Parsing: '" << filename << "'..." << endl;
		// create new session
		SessionInfo tempSession = SessionInfo();
		// read data from file
		ReadTheData(filename, tempSession);
		// push the session into vector
		si.push_back(tempSession);
	}

	cout << endl;
}


// function to process the input file and read the data into the vector
bool ReadTheData(std::string filename,
	SessionInfo &sessionInfo) {

	// create ifstream
	ifstream file(filename);
	string line;
	std::vector<char> correctAnswers;

	// verify if file opened succesfully
	if (!file.good()) {
		cout << " Cannot open file." << endl;
		return false;		// file open failed
	}

	// process the file and read into the vector
	// find the line with session ID
	while (getline(file, line)) {
		if (line.find("<ssn") == string::npos)
			continue;
		else
			break;
	}

	// sets the session informations for current session
	sessionInfo.SetSessionID(findSessionID(line));
	sessionInfo.SetDate(getDateFromFileName(filename));
	sessionInfo.SetTime(getTimeFromFileName(filename));

	// process the line with the question
	while (getline(file, line)) {
		if (line.find("<p") != string::npos) {
			// stopped at question line
			// update questions count
			sessionInfo.updateNumOfQuestions(1);
			// grab correct answer
			correctAnswers = getCorrectAnswers(line);
			// process answers
			while (getline(file, line)) {
				// process student data
				if (line.find("<v") != string::npos) {
					updateStudentInfo(line, correctAnswers, sessionInfo);
				}
				else {
					break;	//break from loop
				}
			}
		}
		else {
			continue;		// continue
		}
	}

	sessionInfo.updateNumOfClickers(sessionInfo.GetStudents().size());

	return true;	// input processed succesfully
}


// find the session id in the line and returns it
string findSessionID(string line) {

	string sessionID;

	// get the line legth
	int lineLength = line.length();
	// find where the "ssnn" string begins
	std::size_t ssnn = line.find("ssnn");
	// forward to the session name
	ssnn += 6;
	// get the remaining line
	string myLine = line.substr(ssnn, lineLength);
	// parse the line t get session id
	stringstream ss(myLine);
	getline(ss, sessionID, '"');

	return sessionID;		// return session id
}

// generates string with date
string getDateFromFileName(string filename) {

	// initialize date
	string date = "XXXXXXXX";

	//extract the digits and combine into time string
	//month
	date[0] = filename[3];
	date[1] = filename[4];
	date[2] = '/';
	// day
	date[3] = filename[5];
	date[4] = filename[6];
	date[5] = '/';
	// year
	date[6] = filename[1];
	date[7] = filename[2];

	return date;
}

// generate string with time
string getTimeFromFileName(string filename) {

	// initialize time
	string time = "XXXXX";

	//extract the digits and combine into time string
	time[0] = filename[7];
	time[1] = filename[8];
	time[2] = ':';
	time[3] = filename[9];
	time[4] = filename[10];

	return time;
}

// collect possible correct answers
std::vector<char> getCorrectAnswers(string line) {

	// correct answers
	std::vector<char> anws;
	// get the line legth
	int lineLength = line.length();
	// find where the "cans" string begins
	std::size_t cans = line.find("cans");
	// forward to the answer
	cans += 6;
	// push the the answer to the vector
	anws.push_back(line[cans]);

	// check for the next answer can be up to 5
	// go to next letter
	cans++;
	while (line[cans] == '|') {
		cans++;
		anws.push_back(line[cans]);
		cans++;
	}

	return anws;		// return answers
}

// updates student info in the vector
void updateStudentInfo(string line, std::vector<char> correctAnswers,
	SessionInfo &si) {

	char studentAnswer = 'X';
	// get the line legth
	int lineLength = line.length();
	// find where the " ans" string begins
	std::size_t studentAnsLocationInLine = line.find(" ans");
	// forward to the answer
	studentAnsLocationInLine += 6;
	// assign the answer
	studentAnswer = line[studentAnsLocationInLine];

	// grab the iClicker ID
	string clickerID;
	// get the line lenght
	lineLength = line.length();
	// find where the ID begins
	std::size_t studentClickerLocationInLine = line.find("id=");
	// forward to the id
	studentClickerLocationInLine += 5;
	// get the remaining line
	string myLine = line.substr(studentClickerLocationInLine, lineLength);
	// parse the line to get clicker id
	stringstream ss(myLine);
	getline(ss, clickerID, '"');

	// update
	findAndUpdatedStudent(studentAnswer, correctAnswers, clickerID, si);
}

// find the student and update, add if not found
void findAndUpdatedStudent(char studentAnswer, std::vector<char> correctAnswers,
	string clickerID, SessionInfo &si) {


	// check if the student already on the list
	auto iter = find_if(si.GetStudentsBeginning(), si.GetStudentsEnding(), [=](Student s)
	{
		return s.GetclickerID() == clickerID;
	});

	// push the student into vector
	if (iter == si.GetStudentsEnding()) {
		Student st = Student(clickerID, 0, 0);
		si.AddStudent(st);
	}

	// correct answer, find student,
	// update correct and totalAnswers
	if (answeredCorrectly(studentAnswer, correctAnswers)) {
		auto temp = find_if(si.GetStudentsBeginning(), si.GetStudentsEnding(), [=](Student s)
		{
			return s.GetclickerID() == clickerID;
		});

		//cout << "correct: " << endl;

		temp->UpdateQuestionsAnsweredCorrectly(1);
		temp->UpdateQuestionsAnsweredTotal(1);
	}
	// incorrect answer, update only total answers
	else if (studentAnswer != '"') {
		auto temp = find_if(si.GetStudentsBeginning(), si.GetStudentsEnding(), [=](Student s)
		{
			return s.GetclickerID() == clickerID;
		});
		//cout << "not correct: " << endl;
		temp->UpdateQuestionsAnsweredTotal(1);
	}
	// if that line is reached
	// student did not answered, no need to update
	else {
		return;
	}
}

// returns true if student answered correctly, false otherwise
bool answeredCorrectly(char studentAns, std::vector<char> correctAnws) {

	// loop through all answers
	for (auto ans : correctAnws) {
		if (studentAns == ans) {
			return true;		// correct answer
		}
	}

	return false;				// incorrect answer

}

// display the statistics about the session
void showStatistics(std::vector<Student> &s, SessionInfo &si) {

	// show session stats
	si.showStats();

	// show student stats who answered the question,
	// correct or incorrect answer
	showStudentsWhoAnsweredTheQuestions(s, si);

	// show student stats who answered correctly all questions
	showStudentsWhoAnsweredTheQuestionsCorrectly(s, si);

	// list students with less than a half
	listStudentsWithLessThanHalfCorrect(s, si);

	// list students with 0 correct
	listStudentsWithZeroCorrect(s, si);

}

// answered correctly or incorectly
void showStudentsWhoAnsweredTheQuestions(std::vector<Student> s, SessionInfo si) {

	cout << "# of students who answered:" << endl;
	cout << "  All questions: " << numOfStudentsAnsweredAllQuestions(s, si) << endl;
	cout << "  At least half: " << numOfStudentsAnsweredAtLeastHalfQuestions(s, si) << endl;
	cout << "  At least one: " << numOfStudentsAnsweredAtLeastOneQuestions(s, si) << endl;
	cout << "  None: " << numOfStudentsAnsweredZeroQuestions(s, si) << endl;
}

// numbers of student who answered the question
// ever correctly or not
int numOfStudentsAnsweredAllQuestions(std::vector<Student> s, SessionInfo si) {

	// get the total questions asked
	int totalQuestions = si.GetNumOfQuestions();

	// ccount the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return s.GetQuestionsAnsweredTotal() == totalQuestions;
	});
}

// number of students answered at least half of the questions
int numOfStudentsAnsweredAtLeastHalfQuestions(std::vector<Student> s, SessionInfo si) {

	// determine the half threshold
	int halfQuestions = 0;
	if ((si.GetNumOfQuestions() % 2) == 0)
		halfQuestions = si.GetNumOfQuestions() / 2;
	else
		halfQuestions = (si.GetNumOfQuestions() / 2) + 1;

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return (s.GetQuestionsAnsweredTotal()) >= halfQuestions;
	});
}

// number of students answered at least one of the questions
int numOfStudentsAnsweredAtLeastOneQuestions(std::vector<Student> s, SessionInfo si) {

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return (s.GetQuestionsAnsweredTotal()) >= 1;
	});
}

// number of students answered 0 questions
int numOfStudentsAnsweredZeroQuestions(std::vector<Student> s, SessionInfo si) {

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return (s.GetQuestionsAnsweredTotal()) == 0;
	});
}

// answered correctly statistics
void showStudentsWhoAnsweredTheQuestionsCorrectly(std::vector<Student> s, SessionInfo si) {

	// displays particular statistics about the session
	cout << "# of students who answered correctly:" << endl;
	cout << "  All questions: " << numOfStudentsAnsweredAllQuestionsCorrectly(s, si) << endl;
	cout << "  At least half: " << numOfStudentsAnsweredAtLeastHalfQuestionsCorrectly(s, si) << endl;
	cout << "  At least one: " << numOfStudentsAnsweredAtLeastOneQuestionsCorrectly(s, si) << endl;
	cout << "  None: " << numOfStudentsAnsweredZeroQuestionsCorrectly(s, si) << endl;
}

// numbers of student who answered the questions correctly
int numOfStudentsAnsweredAllQuestionsCorrectly(std::vector<Student> s, SessionInfo si) {

	// get the total questions asked
	int totalQuestions = si.GetNumOfQuestions();

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return s.GetQuestionsAnsweredCorrectly() == totalQuestions;
	});
}

// count the students who answered at least half questions correctly
int numOfStudentsAnsweredAtLeastHalfQuestionsCorrectly(std::vector<Student> s, SessionInfo si) {

	// determine the half threshold
	int halfQuestions = 0;
	if ((si.GetNumOfQuestions() % 2) == 0)
		halfQuestions = si.GetNumOfQuestions() / 2;
	else
		halfQuestions = (si.GetNumOfQuestions() / 2) + 1;

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return s.GetQuestionsAnsweredCorrectly() >= halfQuestions;
	});
}

// count the students with at least one correct answer
int numOfStudentsAnsweredAtLeastOneQuestionsCorrectly(std::vector<Student> s, SessionInfo si) {

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return s.GetQuestionsAnsweredCorrectly() >= 1;
	});
}

// count the students with 0 correct answers
int numOfStudentsAnsweredZeroQuestionsCorrectly(std::vector<Student> s, SessionInfo si) {

	// count the students
	return count_if(s.begin(), s.end(), [=](Student s) {
		return s.GetQuestionsAnsweredCorrectly() == 0;
	});
}

// displays clicker ID of the students who got less than half correct answers
void listStudentsWithLessThanHalfCorrect(std::vector<Student> s, SessionInfo si) {

	// determine the half threshold
	int halfQuestions = 0;
	if ((si.GetNumOfQuestions() % 2) == 0)
		halfQuestions = si.GetNumOfQuestions() / 2;
	else
		halfQuestions = (si.GetNumOfQuestions() / 2) + 1;

	// display each student
	cout << "Students who answered < half: " << endl;
	for (auto student : s) {
		if (student.GetQuestionsAnsweredTotal() < halfQuestions) {
			cout << "  " << student.GetclickerID() << endl;
		}
	}
}

// displays clicker ID of the students who got less 0 correct answers
void listStudentsWithZeroCorrect(std::vector<Student> s, SessionInfo si) {

	// displays each student
	cout << "Students with 0 correct: " << endl;
	for (auto student : s) {
		if (student.GetQuestionsAnsweredCorrectly() == 0) {
			cout << "  " << student.GetclickerID() << endl;
		}
	}
}
