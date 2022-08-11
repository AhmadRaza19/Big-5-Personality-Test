// Ahmad Raza CS 251
// Project 2 - Personality Tests
// Description : header function containing helper
// functions called in main
// Creative Component : Anime characters

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cstdlib>
//#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    //int ind = randomInteger(0, (int)questions.size()-1);
    int ind = rand() % (int)questions.size();
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

// Returns random question from set
Question randomQuestionFrom(set<Question>& questions) {
    if (questions.size() == 0) {
        throw runtime_error("set: empty");
    }
    struct Question ret = randomElement(questions);
    questions.erase(ret);
    return ret;
}

// Returns new map with scores taken from questions
map<char, int> scoresFrom(map<Question, int>& answers) {
    map <char, int> ret;
    for (auto &q : answers) {
        int num = q.second - 3;
        for (auto &s : q.first.factors) {
            char tempchar = s.first;
            int tempval = s.second * num;
            if (ret.count(tempchar) == 1) {
                ret[tempchar] = ret[tempchar] + tempval;
            } else {
                ret.emplace(tempchar, tempval);
            }
        }
    }
    return ret;
}

// Normalizes scores in a map of integers and factors
map<char, double> normalize(map<char, int>& scores) {
    map <char, double> ret;
    vector <int> temp;
    double norm = 0;
    for (auto &s : scores) {
       temp.push_back(s.second);
    }
    for (int i = 0; i < temp.size(); i++) {
        norm = norm + (temp[i] * temp[i]);
    }
    norm = sqrt(norm);
    
    if (norm == 0) {
        throw runtime_error("normalized factor can't be 0");
    } else {
        for (auto &s : scores) {
            char tempchar = s.first;
            double tempval = s.second / norm;
            ret.emplace(tempchar, tempval);
        }
    }
    return ret;
}

// Calculates the cosine cosine similarity 
// between 2 maps of normalized scores
double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
    double retval = 0;
    set <char> factors;
    for (auto &x : lhs) {
        factors.insert(x.first);
    }
    for (auto &x : rhs) {
        factors.insert(x.first);
    }
    for (auto f : factors) {
        if (lhs.count(f) == 1 && rhs.count(f) == 1) {
            double temp = lhs.at(f) * rhs.at(f);
            retval = retval + temp;
        }
    }
    return retval;
}

// takes map of scores and set of people 
// and checks which person is closest to score
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    if (people.size() == 0) {
        throw runtime_error("set: size is empty");
    }
    double tempval;
    double highest = -999;
    struct Person pRet;
    map <char, double> base = normalize(scores);
    map <char, double> check;
    for (auto p : people) {
        check = normalize(p.scores);
        tempval = cosineSimilarityOf(base, check);
        if (tempval > highest) {
            highest = tempval;
            pRet = p;
        }
    }
    return pRet;
}
