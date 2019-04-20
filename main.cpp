// SATpatt (c) Andrew Woo, 2019
// Website: https://wooandrew.github.io

// SAT is a registered trademark of the College Board. 
// It is not involved with, nor does it endorse this program.

/* Title: SATpatt
 * Author: Andrew Woo
 * Version: rA00x
 * License: MIT
 *
 * Purpose: 
 *	The purpose of this program is to streamline the SAT studying process
 *	by acquiring user input, comparing the answers input to previous answers
 *	on the same test, and recognizing/recording patterns of wrong answers.
 *
 * Disclaimer:
 *	SAT is a registered trademark of the College Board. It
 *	is not involved with, nor does it endorse this program.
**/

/* SAT Sections
 *
 *	Reading: 52 Questions
 *	Writing: 44 Questions
 *	Math No Calculator: 20 Questions
 *	Math Calculator:	38 Questions
**/

/* Pseudocode [bA001]:
 *
 *	ask test number
 *	get test number
 *	match test
 *	enter answers into array
 *	ask answers
 *	get answers
 *	compare answers
 *	log answers
 *	find commonalities
**/

/// v0.1.0 ~ bA001 ::: BASIC FUNCTIONALITY.
/// v1.0.0 ~ rA001 ::: ^^^^^^^^^^^^^^^^^^^^
/// v1.1.0 ~ bA002 ::: ADDS BIASED PATTERN OUTPUT.
/// v1.2.0 ~ bA003 ::: ADDS FILE AUTO-GEN.
/// v1.2.0 ~ rA003 ::: ^^^^^^^^^^^^^^^^^^^
/// v1.3.0 ~ bA004 ::: MD5 Hash Checking


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <limits>
#include <iomanip>

#include "fileautogen.h"

// Scoring
void Score(std::string s_TestID, int readingCorrect, int writingCorrect, int mathCorrect) {

	std::vector<int> vRawScore_Reading;
	std::vector<int> vRawScore_Writing;
	std::vector<int> vRawScore_Math;

	std::ifstream inRawScore_Reading("ans\\" + s_TestID + "_reading.rawscore");
	if (inRawScore_Reading.is_open()) {

		std::string temp = "" ;

		while (getline(inRawScore_Reading, temp)) {
			vRawScore_Reading.push_back(stoi(temp));
		}
	}
	inRawScore_Reading.close();

	std::ifstream inRawScore_Writing("ans\\" + s_TestID + "_writing.rawscore");
	if (inRawScore_Writing.is_open()) {

		std::string temp = "";

		while (getline(inRawScore_Writing, temp)) {
			vRawScore_Writing.push_back(stoi(temp));
		}
	}
	inRawScore_Writing.close();

	std::ifstream inRawScore_Math("ans\\" + s_TestID + "_math.rawscore");
	if(inRawScore_Math.is_open()) {

		std::string temp = "";

		while (getline(inRawScore_Math, temp)) {
			vRawScore_Math.push_back(stoi(temp));
		}
	}
	inRawScore_Math.close();

	std::cout << "\nReading # Correct: " << readingCorrect << " ___ Raw Score: " << vRawScore_Reading[readingCorrect] << std::endl;
	std::cout << "Writing # Correct: " << writingCorrect << " ___ Raw Score: " << vRawScore_Writing[writingCorrect] << std::endl;
	std::cout << "Math    # Correct: " << mathCorrect << " ___ Raw Score: " << vRawScore_Math[mathCorrect] << std::endl;
	std::cout << "Test " << s_TestID << " Score: " << ((vRawScore_Reading[readingCorrect] + vRawScore_Writing[writingCorrect]) * 10) + vRawScore_Math[mathCorrect] 
			  << std::endl;
}

// SAT dictionary
class SATDict {

public:

	SATDict() {}
	~SATDict() {}

	void Init(std::string section, int qNum, std::string correctAns) {

		if (section == "reading") {
			vReading[qNum].first = correctAns;
		}
		else if (section == "writing") {
			vWriting[qNum].first = correctAns;
		}
		else if (section == "mathnocalc") {
			vMathNoCalc[qNum].first = correctAns;
		}
		else if (section == "mathcalc") {
			vMathCalc[qNum].first = correctAns;
		}
	}

	void Add(std::string section, int qNum, std::string uAnswer) {

		if (section == "reading") {
			vReading[qNum].second.push_back(uAnswer);
		}
		else if (section == "writing") {
			vWriting[qNum].second.push_back(uAnswer);
		}
		else if (section == "mathnocalc") {
			vMathNoCalc[qNum].second.push_back(uAnswer);
		}
		else if (section == "mathcalc") {
			vMathCalc[qNum].second.push_back(uAnswer);
		}
	}

	std::string GetCorrectAnswer(std::string section, int qNum) {
		
		if (section == "reading") {
			return vReading[qNum].first;
		}
		else if (section == "writing") {
			return vWriting[qNum].first;
		}
		else if (section == "mathnocalc") {
			return vMathNoCalc[qNum].first;
		}
		else if (section == "mathcalc") {
			return vMathCalc[qNum].first;
		}
		else {
			return "E";
		}
	}

	/*void Output() {

		for (unsigned int x = 0; x < vReading.size(); x++) {
			std::cout << vReading[x] << std::endl;
		}
		for (unsigned int x = 0; x < vWriting.size(); x++) {
			std::cout << vWriting[x] << std::endl;
		}
		for (unsigned int x = 0; x < vMathNoCalc.size(); x++) {
			std::cout << vMathNoCalc[x] << std::endl;
		}
		for (unsigned int x = 0; x < vMathCalc.size(); x++) {
			std::cout << vMathCalc[x] << std::endl;
		}
	}*/

private:

	std::vector<std::pair<std::string, std::vector<std::string>>> vReading = std::vector<std::pair<std::string, std::vector<std::string>>>(52);
	std::vector<std::pair<std::string, std::vector<std::string>>> vWriting = std::vector<std::pair<std::string, std::vector<std::string>>>(44);
	std::vector<std::pair<std::string, std::vector<std::string>>> vMathNoCalc = std::vector<std::pair<std::string, std::vector<std::string>>>(20);
	std::vector<std::pair<std::string, std::vector<std::string>>> vMathCalc = std::vector<std::pair<std::string, std::vector<std::string>>>(38);
};

// Main Function
int main() {

	system("TITLE SATpatt");

	FileAutoGen fAutoGen;
	fAutoGen.initSetup();

	bool exit = false;

	std::string fBase_Reading = "_reading.ans";
	std::string fBase_Writing = "_writing.ans";
	std::string fBase_MathNoCalc = "_mathnocalc.ans";
	std::string fBase_MathCalc = "_mathcalc.ans";
	
	std::string fBase_AnsPatterns = "_test.pattern";


	std::cout << "SATpatt (c) Andrew Woo, 2019" << std::endl;
	std::cout << "SAT is a registered trademark of the College Board. It is not involved with, nor does it endorse this program.\n" << std::endl;
	std::cout << "The purpose of this program is to streamline students' SAT studying. By comparing answers, this program finds" << std::endl;
	std::cout << "and helps students to fix/learn from their mistakes, making sure to only display answers that have been answered" << std::endl;
	std::cout << "wrong more than once. Users are expected to have taken each official SAT practice test more than once." << std::endl;

	while (!exit) {

		SATDict dict;

		int testQuestionNum = 0;

		int testID = 0;
		std::string s_TestID = "";

		int readingCorrect = 0;
		int writingCorrect = 0;
		int mathCorrect = 0;

		std::ifstream inAnsReading;
		std::ifstream inAnsWriting;
		std::ifstream inAnsMathNoCalc;
		std::ifstream inAnsMathCalc;

		std::ifstream inAnsPatterns;
		std::ofstream outAnsPatterns;

		std::vector<int> v_ReadingWrongAns;
		std::vector<int> v_WritingWrongAns;
		std::vector<int> v_MathNoCalcWrongAns;
		std::vector<int> v_MathCalcWrongAns;
		std::vector<int> v_TestAnsPatterns = std::vector<int>(154);

		system("TITLE SATpatt");

		// Match Test
		std::cout << "\nEnter 'exit' to close program. Enter 'U' for unanswered questions.";
		std::cout << "\nEnter Test # [01, 02, 03, etc.]: ";
		std::cin >> s_TestID;

		if (s_TestID == "exit") {
			exit = true;
			continue;
		}

		try {
			testID = std::stoi(s_TestID);
		}
		catch (std::invalid_argument) {
			std::cerr << "Error: Bad input. Please enter a number." << std::endl;
			continue;
		}

		system("TITLE SATpatt ~ Loading test...");

		if (testID == 1) {
			std::cout << "Loading test: 01 . ";
			s_TestID = "01";
		}
		else if (testID == 2) {
			std::cout << "Loading test: 02 . ";
			s_TestID = "02";
		}
		else if (testID == 3) {
			std::cout << "Loading test: 03 . ";
			s_TestID = "03";
		}
		else if (testID == 4) {
			std::cout << "Loading test: 04 . ";
			s_TestID = "04";
		}
		else if (testID == 5) {
			std::cout << "Loading test: 05 . ";
			s_TestID = "05";
		}
		else if (testID == 6) {
			std::cout << "Loading test: 06 . ";
			s_TestID = "06";
		}
		else if (testID == 7) {
			std::cout << "Loading test: 07 . ";
			s_TestID = "07";
		}
		else if (testID == 8) {
			std::cout << "Loading test: 08 . ";
			s_TestID = "08";
		}
		else {

			std::cerr << "Error: Unknown test. Please try again." << std::endl;
			testID = 0;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		
		// Load Answers
		std::string temp = "";

		inAnsReading.open("ans\\" + s_TestID + fBase_Reading);
		if (inAnsReading.is_open()) {

			int iter = 0;

			while (getline(inAnsReading, temp)) {
				dict.Init("reading", iter, temp);
				iter++;
			}
		}
		inAnsReading.close();
		std::cout << ". ";

		inAnsWriting.open("ans\\" + s_TestID + fBase_Writing);
		if (inAnsWriting.is_open()) {

			int iter = 0;

			while (getline(inAnsWriting, temp)) {
				dict.Init("writing", iter, temp);
				iter++;
			}
		}
		inAnsWriting.close();
		std::cout << ". ";

		inAnsMathNoCalc.open("ans\\" + s_TestID + fBase_MathNoCalc);
		if (inAnsMathNoCalc.is_open()) {

			int iter = 0;

			while (getline(inAnsMathNoCalc, temp)) {
				dict.Init("mathnocalc", iter, temp);
				iter++;
			}
		}
		inAnsMathNoCalc.close();
		std::cout << ". ";

		inAnsMathCalc.open("ans\\" + s_TestID + fBase_MathCalc);
		if (inAnsMathCalc.is_open()) {

			int iter = 0;

			while (getline(inAnsMathCalc, temp)) {
				dict.Init("mathcalc", iter, temp);
				iter++;
			}
		}
		inAnsMathCalc.close();
		std::cout << ". ";

		inAnsPatterns.open("ans\\" + s_TestID + fBase_AnsPatterns);
		if (inAnsPatterns.is_open()) {

			int iter = 0;

			while (getline(inAnsPatterns, temp)) {
				v_TestAnsPatterns[iter] = std::stoi(temp);
				iter++;
			}
		}
		inAnsPatterns.close();
		std::cout << ". . ";

		std::cout << "DONE\n" << std::endl;

		// Get Answers
		std::string tempAns = "";

		system("TITLE SATpatt ~ Enter answers...");
		for (int x = 0; x < 52; x++, testQuestionNum++) { // Section: Reading

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Reading Question " << x + 1 << ": ";
			std::cin >> tempAns;

			tempAns = toupper(tempAns[0]);

			if (tempAns == "A" || tempAns == "B" || tempAns == "C" || tempAns == "D" || tempAns == "U") {
				
				dict.Add("reading", x, tempAns);

				if (dict.GetCorrectAnswer("reading", x) == tempAns) {
					readingCorrect++;
				}
				else {
					v_ReadingWrongAns.push_back(x + 1);
					v_TestAnsPatterns[testQuestionNum]++;
				}
			}
			else {
				std::cout << "Error: Unrecognized answer [" << tempAns << "]. Please try again." << std::endl;
				x--;
				testQuestionNum--;
			}
		}

		for (int x = 0; x < 44; x++, testQuestionNum++) { // Section: Writing

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Writing Question " << x + 1 << ": ";
			std::cin >> tempAns;

			tempAns = toupper(tempAns[0]);

			if (tempAns == "A" || tempAns == "B" || tempAns == "C" || tempAns == "D" || tempAns == "U") {
				
				dict.Add("writing", x, tempAns);

				if (dict.GetCorrectAnswer("writing", x) == tempAns) {
					writingCorrect++;
				}
				else {
					v_WritingWrongAns.push_back(x + 1);
					v_TestAnsPatterns[testQuestionNum]++;
				}
			}
			else {
				std::cout << "Error: Unrecognized answer [" << tempAns << "]. Please try again." << std::endl;
				x--;
				testQuestionNum--;
			}
		}

		for (int x = 0; x < 15; x++, testQuestionNum++) { // Section: Math - No Calculator .. Multiple Choice

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Math - No Calculator Question " << x + 1 << ": ";
			std::cin >> tempAns;

			tempAns = toupper(tempAns[0]);

			if (tempAns == "A" || tempAns == "B" || tempAns == "C" || tempAns == "D" || tempAns == "U") {
				
				dict.Add("mathnocalc", x, tempAns);

				if (dict.GetCorrectAnswer("mathnocalc", x) == tempAns) {
					mathCorrect++;
				}
				else {
					v_MathNoCalcWrongAns.push_back(x + 1);
					v_TestAnsPatterns[testQuestionNum]++;
				}
			}
			else {
				std::cout << "Error: Unrecognized answer [" << tempAns << "]. Please try again." << std::endl;
				x--;
				testQuestionNum--;
			}
		}
		for (int x = 0; x < 5; x++, testQuestionNum++) { // Section: Math - No Calculator .. Long Answer

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Math - No Calculator Question " << x + 16 << ": ";
			std::cin >> tempAns;
		
			if (toupper(tempAns[0]) == 85) {

				v_MathNoCalcWrongAns.push_back(x + 16);
				v_TestAnsPatterns[testQuestionNum]++;

				continue;
			}

			bool correctAnswer = false;

			char buf[50];
			std::stringstream ss(dict.GetCorrectAnswer("mathnocalc", x + 15));
			bool fraction = false;

			for (unsigned int z = 0; z < tempAns.size(); z++) {

				if (tempAns[z] == 47) {
					fraction = true;
					break;
				}
			}

			if (fraction) {

				while (ss >> buf) {

					if (buf == tempAns) {
						mathCorrect++;
						correctAnswer = true;
					}
				}
			}
			else {
				
				while (ss >> buf) {

					if (std::stof(buf) == std::stof(tempAns)) {
						mathCorrect++;
						correctAnswer = true;
					}
				}
			}

			if(!correctAnswer) {
				v_MathNoCalcWrongAns.push_back(x + 16);
				v_TestAnsPatterns[testQuestionNum]++;
			}
		}

		for (int x = 0; x < 30; x++, testQuestionNum++) { // Section: Math - Calculator .. Multiple Choice

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Math - Calculator Question " << x + 1 << ": ";
			std::cin >> tempAns;

			tempAns = toupper(tempAns[0]);

			if (tempAns == "A" || tempAns == "B" || tempAns == "C" || tempAns == "D" || tempAns == "U") {
				
				dict.Add("mathcalc", x, tempAns);

				if (dict.GetCorrectAnswer("mathcalc", x) == tempAns) {
					mathCorrect++;
				}
				else {
					v_MathCalcWrongAns.push_back(x + 1);
					v_TestAnsPatterns[testQuestionNum]++;
				}
			}
			else {
				std::cout << "Error: Unrecognized answer [" << tempAns << "]. Please try again." << std::endl;
				x--;
				testQuestionNum--;
			}
		}
		for (int x = 0; x < 8; x++, testQuestionNum++) { // Section: Math - Calculator .. Long Answer

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Math - Calculator Question " << x + 31 << ": ";
			std::cin >> tempAns;

			if (toupper(tempAns[0]) == 85) {

				v_MathCalcWrongAns.push_back(x + 31);
				v_TestAnsPatterns[testQuestionNum]++;

				continue;
			}

			bool correctAnswer = false;

			char buf[50];
			std::stringstream ss(dict.GetCorrectAnswer("mathcalc", x + 30));
			bool fraction = false;

			for (unsigned int z = 0; z < tempAns.size(); z++) {

				if (tempAns[z] == 47) {
					fraction = true;
					break;
				}
			}

			if (fraction) {

				while (ss >> buf) {

					if (buf == tempAns) {
						mathCorrect++;
						correctAnswer = true;
					}
				}
			}
			else {

				while (ss >> buf) {

					if (std::stof(buf) == std::stof(tempAns)) {
						mathCorrect++;
						correctAnswer = true;
					}
				}
			}

			if (!correctAnswer) {
				v_MathCalcWrongAns.push_back(x + 31);
				v_TestAnsPatterns[testQuestionNum]++;
			}
		}

		system("TITLE SATpatt ~ Your SAT Score...");
		Score(s_TestID, readingCorrect, writingCorrect, mathCorrect);
		
		std::cout << std::endl;
		system("PAUSE");
		std::cout << std::endl;

		system("TITLE SATpatt ~ Wrong Answers...");
		std::cout << "Incorrect Answers:\nReading: ";
		for (unsigned int x = 0; x < v_ReadingWrongAns.size(); x++) {
			std::cout << v_ReadingWrongAns[x] << " ";
		}

		std::cout << "\nWriting: ";
		for (unsigned int x = 0; x < v_WritingWrongAns.size(); x++) {
			std::cout << v_WritingWrongAns[x] << " ";
		}

		std::cout << "\nMath - No Calculator: ";
		for (unsigned int x = 0; x < v_MathNoCalcWrongAns.size(); x++) {
			std::cout << v_MathNoCalcWrongAns[x] << " ";
		}

		std::cout << "\nMath - Calculator: ";
		for (unsigned int x = 0; x < v_MathCalcWrongAns.size(); x++) {
			std::cout << v_MathCalcWrongAns[x] << " ";
		}

		std::cout << '\n' << std::endl;
		system("PAUSE");
		std::cout << std::endl;

		system("TITLE SATpatt ~ Pattern...");
		std::cout << "Questions answered incorrectly multiple times: " << std::endl;
		outAnsPatterns.open("ans//" + s_TestID + fBase_AnsPatterns);
		for (unsigned int x = 0; x < v_TestAnsPatterns.size(); x++) {

			if (v_TestAnsPatterns[x] > 1) {

				if (x < 53) { // Reading

					for (unsigned int z = 0; z < v_ReadingWrongAns.size(); z++) {
						
						if (x + 1 == v_ReadingWrongAns[z]) {
							std::cout << "Reading Question     [" << std::setfill('0') << std::setw(2) << x + 1 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << v_TestAnsPatterns[x] << "] times." << std::endl;
						}
					}
				}
				else if (x < 97) { // Writing

					for (unsigned int z = 0; z < v_WritingWrongAns.size(); z++) {

						if (x - 51 == v_WritingWrongAns[z]) {
							std::cout << "Writing Question     [" << std::setfill('0') << std::setw(2) << x - 51 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << v_TestAnsPatterns[x] << "] times." << std::endl;
						}
					}
				}
				else if (x < 117) { // Math - No Calculator

					for (unsigned int z = 0; z < v_MathNoCalcWrongAns.size(); z++) {

						if (x - 95 == v_MathNoCalcWrongAns[z]) {
							std::cout << "Math - No Calculator [" << std::setfill('0') << std::setw(2) << x - 95 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << v_TestAnsPatterns[x] << "] times." << std::endl;
						}
					}
				}
				else { // Math - Calculator

					for (unsigned int z = 0; z < v_MathCalcWrongAns.size(); z++) {

						if (x - 115 == v_MathCalcWrongAns[z]) {
							std::cout << "Math - Calculator    [" << std::setfill('0') << std::setw(2) << x - 115 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << v_TestAnsPatterns[x] << "] times." << std::endl;
						}
					}
				}
			}

			outAnsPatterns << v_TestAnsPatterns[x] << std::endl;
		}
		outAnsPatterns.close();

		std::cout << std::endl;
		system("PAUSE");
		std::cout << std::endl;
		
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return 0;
}
