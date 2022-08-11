// Ahmad Raza CS 251
// Project 2 - Personality Tests
// Description : App that outputs basic interface
// which allows user to answer questions and match
// Creative Component : Anime characters

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;

// Helper function for checkMap which
// adds factors from questions.txt into map
void factorCheck(int i, char factor, string temp, int &score, map <char, int> &m) {
    if (i + 2 < temp.size()) {
        if (temp[i+2] - '0' == 1) {
            score = 1;
            m.emplace(factor, score);
        } else if (temp[i+2] == '-') {
            score = -1;
            m.emplace(factor, score);
        }
    }
}

// Helper function for scoreCheckHelper which
// deals with negative number cases for .people files
void checkNeg (int i, char factor, string temp, int &score, map <char, int> &m) {
    if (isdigit(temp[i+4])) {
        score = (((temp[i+3] - '0') * 10) + (temp[i+4] - '0')) * -1;
        m.emplace(factor, score);
    } else {
        score = (temp[i+3] - '0') * -1;
        m.emplace(factor, score);
    }
}

// Helper function for scoreCheck which
// deals with either negative numbers,
// double digits, or both
void scoreCheckHelper(int i, char factor, string temp, int &score, map <char, int> &m) {
    if (isdigit(temp[i + 2])) {
        score = ((temp[i+2] - '0') * 10) + (temp[i+3] - '0');
        m.emplace(factor, score);
        return;
    } else if (temp[i+2] == '-') {
        if (i + 4 < temp.size()) {
            checkNeg(i, factor, temp, score, m);
        }
        else {
            score = (temp[i+3] - '0') * -1;
            m.emplace(factor, score);
        }
        return;
    }
}

// Helper function for checkMap which
// adds scores into map
void scoreCheck(int i, char factor, string temp, int &score, map <char, int> &m) {
    // Checks for numbers which have a total of
    // 3 or more characters (neg, double digits)
    if (i + 3 < temp.size()) {
        if (isdigit(temp[i+3])) {
            scoreCheckHelper(i, factor, temp, score, m);
            return;
        }
    }
    // Checks for numbers that are pos single digits
    if (i + 2 < temp.size()) {
        if (isdigit(temp[i+2])) {
            score = temp[i+2] - '0';
            m.emplace(factor, score);
        }
    }
}

// Helper function for main which
// checks a map and adds factors/scores
void checkMap(char factor, int score, string temp, map <char, int> &m, int which) {
    for (int i = 0; i < temp.size(); i++) {
        if (isalpha(temp[i])) {
            factor = temp[i];
            if (which == 0) {
                factorCheck(i, factor, temp, score, m);
            } else {
                scoreCheck(i, factor, temp, score, m);
            }
        }
    }
}

// Helper function which clears struct
void clearPStruct(Person &p) {
    p.name.clear();
    p.scores.clear();
}


// Helper function which loads test/people files
void loadT(set <Person> &people, string testFile) {
    ifstream fileT;
    fileT.open(testFile);
    string name;
    string temp;
    char factor;
    int score;
    Person p1;
    int which = 1;
    while (!fileT.eof()) {
        getline(fileT, name, '.');
        if (name.size() == 0) {
            continue;
        }
        getline(fileT, temp);

        p1.name = name;

        temp.erase(0, 1);
        checkMap(factor, score, temp, p1.scores, which);
        people.insert(p1);
        clearPStruct(p1);
    }
    fileT.close();
}

// Helper function for main which
// sets file name based on user choice
void chooseTestOption(int choice, string &testFile) {
    switch (choice) {
        case 1: testFile = "BabyAnimals.people"; break;
        case 2: testFile = "Brooklyn99.people"; break;
        case 3: testFile = "Disney.people"; break;
        case 4: testFile = "Hogwarts.people"; break;
        case 5: testFile = "MyersBriggs.people"; break;
        case 6: testFile = "SesameStreet.people"; break;
        case 7: testFile = "StarWars.people"; break;
        case 8: testFile = "Vegetables.people"; break;
        case 9: testFile = "mine.people"; break;
        default : cout << "Please enter a valid number (1-9)" << endl;
    }
}

// Helper function for main which
// prints out test options
void printTestOptions() {
    cout << "1. BabyAnimals" << endl;
    cout << "2. Brooklyn99" << endl;
    cout << "3. Disney" << endl;
    cout << "4. Hogwarts" << endl;
    cout << "5. MyersBriggs" << endl;
    cout << "6. SesameStreet" << endl;
    cout << "7. StarWars" << endl;
    cout << "8. Vegetables" << endl;
    cout << "9. mine" << endl;
    cout << "0. To end program." << endl << endl;
    cout << "Choose test number (1-9, or 0 to end): ";
}

// Helper function for main which
// prints out questions
void printQ(set <Question> questions, map <Question, int> &answers) {
    cout << "Welcome to the Personality Quiz!" << endl << endl;
    cout << "Choose number of questions: ";
    int numQ;
    int choice;
    cin >> numQ;
    cout << endl;
    for (int i = numQ; i > 0; i--) {
        cout << "How much do you agree with this statement?" << endl;
        Question q = randomQuestionFrom(questions);
        cout << "\"" << q.questionText << "\"" << endl << endl;
        cout << "1. Strongly disagree" << endl;
        cout << "2. Disagree" << endl;
        cout << "3. Neutral" << endl;
        cout << "4. Agree" << endl;
        cout << "5. Strongly agree" << endl << endl;
        cout << "Enter your answer here (1-5): ";
        cin >> choice;
        cout << endl;
        answers.emplace(q, choice);
    }
}

// Helper function which clears struct
void clearQStruct(Question &q) {
    q.questionText.clear();
    q.factors.clear();
}

// Helper function for main which
// loads questions from questions.txt
void loadQ(set <Question> &questions) {
    ifstream fileQ;
    fileQ.open("questions.txt");
    string q;
    string temp;
    char factor;
    int score;
    int which = 0;
    Question q1;
    while (!fileQ.eof()) {
        getline(fileQ, q, '.');
        if (q.size() == 0) {
            continue;
        }
        getline(fileQ, temp);

        q.push_back('.');
        q1.questionText = q;

        temp.erase(0, 1);
        checkMap(factor, score, temp, q1.factors, which);
        questions.insert(q1);
        clearQStruct(q1);
    }
    fileQ.close();
}

int main() {
    set <Question> questions;
    loadQ(questions);

    map <Question, int> answers;
    printQ(questions, answers);

    int test;
    printTestOptions();
    cin >> test;
    string testFile;
    
    // Loops until user wants to quit
    while (test != 0) {
        chooseTestOption(test, testFile);
        set <Person> people;

        loadT(people, testFile);
        map <char, int> scores = scoresFrom(answers);
        Person matched = mostSimilarTo(scores, people);

        cout << "You got " << matched.name << "!" << endl << endl;

        printTestOptions();
        cin >> test;
    }
    cout << "Goodbye!" << endl;
    return 0;
}
