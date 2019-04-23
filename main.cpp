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


#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#define NOMINMAX

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <limits>
#include <iomanip>
#include <codecvt>

#include <windows.h>
#include <wincrypt.h>

#include "fileautogen.h"


#define BUFSIZE 1024
#define MD5LEN  16

// Scores SAT practice test
void Score(std::string s_TestID, int readingCorrect, int writingCorrect, int mathCorrect) {

	std::vector<int> vRawScore_Reading;
	std::vector<int> vRawScore_Writing;
	std::vector<int> vRawScore_Math;

	std::ifstream inRawScore_Reading("ans\\" + s_TestID + "_reading.rawscore");
	if (inRawScore_Reading.is_open()) {

		std::string temp = "" ;

		while (getline(inRawScore_Reading, temp)) {
			vRawScore_Reading.push_back(std::stoi(temp));
		}
	}
	inRawScore_Reading.close();

	std::ifstream inRawScore_Writing("ans\\" + s_TestID + "_writing.rawscore");
	if (inRawScore_Writing.is_open()) {

		std::string temp = "";

		while (getline(inRawScore_Writing, temp)) {
			vRawScore_Writing.push_back(std::stoi(temp));
		}
	}
	inRawScore_Writing.close();

	std::ifstream inRawScore_Math("ans\\" + s_TestID + "_math.rawscore");
	if(inRawScore_Math.is_open()) {

		std::string temp = "";

		while (getline(inRawScore_Math, temp)) {
			vRawScore_Math.push_back(std::stoi(temp));
		}
	}
	inRawScore_Math.close();

	std::cout << "\nReading # Correct: " << readingCorrect << " ___ Raw Score: " << vRawScore_Reading[readingCorrect] << std::endl;
	std::cout << "Writing # Correct: " << writingCorrect << " ___ Raw Score: " << vRawScore_Writing[writingCorrect] << std::endl;
	std::cout << "Math    # Correct: " << mathCorrect << " ___ Raw Score: " << vRawScore_Math[mathCorrect] << std::endl;
	std::cout << "Test " << s_TestID << " Score: " << ((vRawScore_Reading[readingCorrect] + vRawScore_Writing[writingCorrect]) * 10) + vRawScore_Math[mathCorrect] 
			  << std::endl;
}

// Contains vector array of correct answers/user answers
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

private:

	std::vector<std::pair<std::string, std::vector<std::string>>> vReading = std::vector<std::pair<std::string, std::vector<std::string>>>(52);
	std::vector<std::pair<std::string, std::vector<std::string>>> vWriting = std::vector<std::pair<std::string, std::vector<std::string>>>(44);
	std::vector<std::pair<std::string, std::vector<std::string>>> vMathNoCalc = std::vector<std::pair<std::string, std::vector<std::string>>>(20);
	std::vector<std::pair<std::string, std::vector<std::string>>> vMathCalc = std::vector<std::pair<std::string, std::vector<std::string>>>(38);
};

// Checks MD5 hash of official College Board SAT practice test files
bool md5check(std::string s_TestID) {

	std::vector<std::string> reading_ans;
	reading_ans.push_back("049c7164bff3223f6b3f627edb0e1916");
	reading_ans.push_back("f5fbaa2179cf2984866da3a87c0bc256");
	reading_ans.push_back("cb8aed6a7e9ad26aa2182eb189e0ce55");
	reading_ans.push_back("9f2e528a779f176229158a7c2e9d6057");
	reading_ans.push_back("20a0db38f5fe29d761dea2849011aa25");
	reading_ans.push_back("3a293f2b9305b459a93c1e61fa9c8b43");
	reading_ans.push_back("e5365783779f7b86d647fa16b44ff01e");
	reading_ans.push_back("1445fa0cc0c477c871cb2b539b49970e");

	std::vector<std::string> writing_ans;
	writing_ans.push_back("52600e3cd1b4740e1497f56bb6bf7bc1");
	writing_ans.push_back("9019cfda9b76a44905de7c1c7e75a9a5");
	writing_ans.push_back("11b297ff52222be6d7645138381c0056");
	writing_ans.push_back("61e0bfc4630b2a96246611ac8b6fcc29");
	writing_ans.push_back("5fbb1718d56948ca8082dcd50eca6083");
	writing_ans.push_back("92db64c226e445d8530f7ab2766aee41");
	writing_ans.push_back("b30be8daa1e79bdd9e0e28d75c215fd1");
	writing_ans.push_back("53d4aed72fa1bce0dc4bd36b6655d3f1");

	std::vector<std::string> mathnocalc_ans;
	mathnocalc_ans.push_back("30f56ec364e2dee94de49f58b5c45e91");
	mathnocalc_ans.push_back("fcb8cdae3750016866011c85ab1bbd95");
	mathnocalc_ans.push_back("1073211754fe14444fcd2aa96b21f3b0");
	mathnocalc_ans.push_back("600df23f2cb899147aca0a82996a1b41");
	mathnocalc_ans.push_back("f330473c411afec8b64ffe69d7e96cf0");
	mathnocalc_ans.push_back("d3e133b7ae93de2650b7565585e7d0d7");
	mathnocalc_ans.push_back("ba840f75f653c373ab4cb5de2e94d58e");
	mathnocalc_ans.push_back("7f5ab2d471e687ab75ed400f1b5862d6");

	std::vector<std::string> mathcalc_ans;
	mathcalc_ans.push_back("6db67fde2358f070e8204a6e96b10e48");
	mathcalc_ans.push_back("95f5eb5bc8dceccacad9b99b3cde1824");
	mathcalc_ans.push_back("030ce43f9195b6764947b2dc4e28ad3e");
	mathcalc_ans.push_back("3b91344287a0d4151861238084254759");
	mathcalc_ans.push_back("325146e05f67feb323ac6032bfcf4cd1");
	mathcalc_ans.push_back("9732fa1518b0f3fdc0280ef5eb803aef");
	mathcalc_ans.push_back("07ab30a65b67d165f82db1a103f15f6e");
	mathcalc_ans.push_back("e4124c05ec3657b2eef3848dda85c9e1");

	std::vector<std::string> reading_rawscore;
	reading_rawscore.push_back("c42cc7af58af0ee80b1654968af7257e");
	reading_rawscore.push_back("6889644023faf7ed5a22998103325bdb");
	reading_rawscore.push_back("875054f3909a4d454f714a0dbcea400a");
	reading_rawscore.push_back("6889644023faf7ed5a22998103325bdb");
	reading_rawscore.push_back("66949ea391433e188dd9353d1544f27c");
	reading_rawscore.push_back("c3953a83aaa9209f9d6d271361dcfced");
	reading_rawscore.push_back("02853f997a2ea8db275c8bb3ade30fe7");
	reading_rawscore.push_back("0a3e397444e14c376924845f091e1205");

	std::vector<std::string> writing_rawscore;
	writing_rawscore.push_back("118f06064993bfd6037b01441a474dde");
	writing_rawscore.push_back("afc899a24da1774ec5eb30c75f6e4db3");
	writing_rawscore.push_back("acf277239884eb0e41f56d705188653e");
	writing_rawscore.push_back("afc899a24da1774ec5eb30c75f6e4db3");
	writing_rawscore.push_back("acd360cb2ae5c64ba04f80b0edd44bab");
	writing_rawscore.push_back("38d30c2ad2fa2bb95e5f64838513a534");
	writing_rawscore.push_back("88b40f1513dc9352cb0b457a41e104aa");
	writing_rawscore.push_back("3ab7e65feee2a3697c13032a6b297864");

	std::vector<std::string> math_rawscore;
	math_rawscore.push_back("60b3c5a0f3be577781b9ef72df6d6327");
	math_rawscore.push_back("f41c09e6d6221d60941357c952666313");
	math_rawscore.push_back("e052b00168f46c7caa9b6a577410cbaa");
	math_rawscore.push_back("f41c09e6d6221d60941357c952666313");
	math_rawscore.push_back("ff7329d26bdd888c1552a82883f39135");
	math_rawscore.push_back("320d54db3e2c8546939a6a088e65a80b");
	math_rawscore.push_back("30f1075ed06a0f3d6d42ed57fb6961e9");
	math_rawscore.push_back("ec4af2ece2552fcd455ea1e72c0a5a92");

	std::vector<std::vector<std::string>> vec_ContainAll;
	vec_ContainAll.push_back(reading_ans);
	vec_ContainAll.push_back(writing_ans);
	vec_ContainAll.push_back(mathnocalc_ans);
	vec_ContainAll.push_back(mathcalc_ans);
	vec_ContainAll.push_back(reading_rawscore);
	vec_ContainAll.push_back(writing_rawscore);
	vec_ContainAll.push_back(math_rawscore);


	int testID = 0;

	std::vector<LPCSTR> fBase;

	std::string ans_sReading = "ans\\" + s_TestID + "_reading.ans";
	std::string ans_sWriting = "ans\\" + s_TestID + "_writing.ans";
	std::string ans_sMathNoCalc = "ans\\" + s_TestID + "_mathnocalc.ans";
	std::string ans_sMathCalc = "ans\\" + s_TestID + "_mathcalc.ans";

	std::string rawscore_sReading = "ans\\" + s_TestID + "_reading.rawscore";
	std::string rawscore_sWriting = "ans\\" + s_TestID + "_writing.rawscore";
	std::string rawscore_sMath = "ans\\" + s_TestID + "_math.rawscore";

	fBase.push_back(ans_sReading.c_str());
	fBase.push_back(ans_sWriting.c_str());
	fBase.push_back(ans_sMathNoCalc.c_str());
	fBase.push_back(ans_sMathCalc.c_str());

	fBase.push_back(rawscore_sReading.c_str());
	fBase.push_back(rawscore_sWriting.c_str());
	fBase.push_back(rawscore_sMath.c_str());

	try {

		if (!(typeid(std::stoi(s_TestID)).name() == "int") && testID <= 8) {
			testID = std::stoi(s_TestID);
		}
	}
	catch (std::invalid_argument) {
		std::cerr << "Error: Invalid Argument. MD5 hash check failed." << std::endl;
		return false;
	}

	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789abcdef";

	for (unsigned int x = 0; x < fBase.size(); x++) {

		hFile = CreateFile(fBase[x], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (INVALID_HANDLE_VALUE == hFile) {

			dwStatus = GetLastError();
			std::cerr << "Error opening file " << fBase[x] << ". Ending MD5check with error " << dwStatus << std::endl;

			return dwStatus;
		}

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {

			dwStatus = GetLastError();
			std::cerr << "CryptAcquireContext failed: " << dwStatus << std::endl;
			CloseHandle(hFile);

			return dwStatus;
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {

			dwStatus = GetLastError();
			std::cerr << "CryptCreateHash failed: " << dwStatus << std::endl;
			CloseHandle(hFile);
			CryptReleaseContext(hProv, 0);

			return dwStatus;
		}

		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL)) {

			if (0 == cbRead) {
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {

				dwStatus = GetLastError();
				std::cerr << "CryptHashData failed: " << dwStatus << std::endl;
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);

				return dwStatus;
			}
		}

		if (!bResult) {

			dwStatus = GetLastError();
			std::cerr << "ReadFile failed: " << dwStatus << std::endl;
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);

			return dwStatus;
		}

		cbHash = MD5LEN;
		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {

			std::stringstream md5hash;
			for (DWORD i = 0; i < cbHash; i++) {
				md5hash << std::hex << rgbDigits[rgbHash[i] >> 4] << std::hex << rgbDigits[rgbHash[i] & 0xf];
			}

			if (vec_ContainAll[x][testID - 1] != md5hash.str().c_str()) {
				std::cerr << "MD5 checksum of " << fBase[x] << " does not match preloaded hash." << std::endl;
				return false;
			}
		}
		else {

			dwStatus = GetLastError();
			std::cerr << "CryptGetHashParam failed: " << dwStatus << std::endl;

			return dwStatus;
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(hFile);
	}

	return true;
}

// Loads test answers/patterns
bool LoadTest(SATDict* dict, std::string s_TestID, std::string fBase_AnsPatterns, std::vector<int>* v_TestAnsPatterns) {

	if (!md5check(s_TestID)) {

		std::cerr << "MD5 check failed" << std::endl;

		return false;
	}

	// Base file names
	std::string fBase_Reading = "_reading.ans";
	std::string fBase_Writing = "_writing.ans";
	std::string fBase_MathNoCalc = "_mathnocalc.ans";
	std::string fBase_MathCalc = "_mathcalc.ans";

	// File read streams
	std::ifstream inAnsReading;
	std::ifstream inAnsWriting;
	std::ifstream inAnsMathNoCalc;
	std::ifstream inAnsMathCalc;

	std::ifstream inAnsPatterns;

	// Load Answers
	std::string temp = "";

	inAnsReading.open("ans\\" + s_TestID + fBase_Reading);
	if (inAnsReading.is_open()) {

		int iter = 0;

		while (getline(inAnsReading, temp)) {
			dict->Init("reading", iter, temp);
			iter++;
		}
	}
	inAnsReading.close();
	std::cout << ". ";

	inAnsWriting.open("ans\\" + s_TestID + fBase_Writing);
	if (inAnsWriting.is_open()) {

		int iter = 0;

		while (getline(inAnsWriting, temp)) {
			dict->Init("writing", iter, temp);
			iter++;
		}
	}
	inAnsWriting.close();
	std::cout << ". ";

	inAnsMathNoCalc.open("ans\\" + s_TestID + fBase_MathNoCalc);
	if (inAnsMathNoCalc.is_open()) {

		int iter = 0;

		while (getline(inAnsMathNoCalc, temp)) {
			dict->Init("mathnocalc", iter, temp);
			iter++;
		}
	}
	inAnsMathNoCalc.close();
	std::cout << ". ";

	inAnsMathCalc.open("ans\\" + s_TestID + fBase_MathCalc);
	if (inAnsMathCalc.is_open()) {

		int iter = 0;

		while (getline(inAnsMathCalc, temp)) {
			dict->Init("mathcalc", iter, temp);
			iter++;
		}
	}
	inAnsMathCalc.close();
	std::cout << ". ";

	inAnsPatterns.open("ans\\" + s_TestID + fBase_AnsPatterns);
	if (inAnsPatterns.is_open()) {

		int iter = 0;

		while (getline(inAnsPatterns, temp)) {
			(*v_TestAnsPatterns)[iter] = std::stoi(temp);
			iter++;
		}
	}
	inAnsPatterns.close();
	std::cout << ". . ";

	std::cout << "DONE\n" << std::endl;

	return true;
}

// Main Function
int main() {

	system("TITLE SATpatt");

	// Create a FileAutoGen object then intialize a setup
	FileAutoGen fAutoGen;
	fAutoGen.initSetup();

	bool exit = false;
	std::ofstream outAnsPatterns;

	// *** Quick Description *********************************************************************************************************************
	std::cout << "SATpatt (c) Andrew Woo, 2019" << std::endl;
	std::cout << "SAT is a registered trademark of the College Board. It is not involved with, nor does it endorse this program.\n" << std::endl;
	std::cout << "The purpose of this program is to streamline students' SAT studying. By comparing answers, this program finds" << std::endl;
	std::cout << "and helps students to fix/learn from their mistakes, making sure to only display answers that have been answered" << std::endl;
	std::cout << "wrong more than once. Users are expected to have taken each official SAT practice test more than once." << std::endl;
	// *** Quick Description *********************************************************************************************************************

	// Main Loop - continue while exit = false
	while (!exit) {

		int testQuestionNum = 0;

		int testID = 0;
		std::string s_TestID;
		std::string fBase_AnsPatterns = "_test.pattern";

		// Initialize a SATDict object
		SATDict* dict = new SATDict;

		// # of correct answers for each section
		int readingCorrect = 0;
		int writingCorrect = 0;
		int mathCorrect = 0;

		std::vector<int> v_ReadingWrongAns;
		std::vector<int> v_WritingWrongAns;
		std::vector<int> v_MathNoCalcWrongAns;
		std::vector<int> v_MathCalcWrongAns;
		std::vector<int>* v_TestAnsPatterns = new std::vector<int>(154);

		system("TITLE SATpatt");

		// Match Test
		std::cout << "\nEnter 'exit' to close program. Enter 'U' for unanswered questions.";
		std::cout << "\nEnter Test # [01, 02, 03, etc.]: ";
		std::cin >> s_TestID;

		// Check if user wants to exit program
		if (s_TestID == "exit") {
			exit = true;
			continue;
		}

		// Convert user input of type std::string to type int
		try {
			testID = std::stoi(s_TestID);
		}
		catch (std::invalid_argument) {
			std::cerr << "Error: Bad input. Please enter a number." << std::endl;
			continue;
		}

		system("TITLE SATpatt ~ Loading test...");

		// Load test
		switch (testID) {
			
		case 1:

			std::cout << "Loading test: 01 . ";
			LoadTest(dict, "01", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 2:

			std::cout << "Loading test: 02 . ";
			LoadTest(dict, "02", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 3:

			std::cout << "Loading test: 03 . ";
			LoadTest(dict, "03", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 4:

			std::cout << "Loading test: 04 . ";
			LoadTest(dict, "04", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 5:

			std::cout << "Loading test: 05 . ";
			LoadTest(dict, "05", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 6:

			std::cout << "Loading test: 06 . ";
			LoadTest(dict, "06", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 7:

			std::cout << "Loading test: 07 . ";
			LoadTest(dict, "07", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		case 8:

			std::cout << "Loading test: 08 . ";
			LoadTest(dict, "08", fBase_AnsPatterns, v_TestAnsPatterns);
			break;

		default:

			std::cerr << "Error: Unknown test. Please try again." << std::endl;
			testID = 0;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		// Get Answers
		std::string tempAns = "";

		system("TITLE SATpatt ~ Enter answers...");
		for (int x = 0; x < 52; x++, testQuestionNum++) { // Section: Reading

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Reading Question " << x + 1 << ": ";
			std::cin >> tempAns;

			tempAns = toupper(tempAns[0]);

			if (tempAns == "A" || tempAns == "B" || tempAns == "C" || tempAns == "D" || tempAns == "U") {
				
				dict->Add("reading", x, tempAns);

				if (dict->GetCorrectAnswer("reading", x) == tempAns) {
					readingCorrect++;
				}
				else {
					v_ReadingWrongAns.push_back(x + 1);
					(*v_TestAnsPatterns)[testQuestionNum]++;
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
				
				dict->Add("writing", x, tempAns);

				if (dict->GetCorrectAnswer("writing", x) == tempAns) {
					writingCorrect++;
				}
				else {
					v_WritingWrongAns.push_back(x + 1);
					(*v_TestAnsPatterns)[testQuestionNum]++;
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
				
				dict->Add("mathnocalc", x, tempAns);

				if (dict->GetCorrectAnswer("mathnocalc", x) == tempAns) {
					mathCorrect++;
				}
				else {
					v_MathNoCalcWrongAns.push_back(x + 1);
					(*v_TestAnsPatterns)[testQuestionNum]++;
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
				(*v_TestAnsPatterns)[testQuestionNum]++;

				continue;
			}

			bool correctAnswer = false;

			char buf[50];
			std::stringstream ss(dict->GetCorrectAnswer("mathnocalc", x + 15));
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
				(*v_TestAnsPatterns)[testQuestionNum]++;
			}
		}

		for (int x = 0; x < 30; x++, testQuestionNum++) { // Section: Math - Calculator .. Multiple Choice

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter your answer for Math - Calculator Question " << x + 1 << ": ";
			std::cin >> tempAns;

			tempAns = toupper(tempAns[0]);

			if (tempAns == "A" || tempAns == "B" || tempAns == "C" || tempAns == "D" || tempAns == "U") {
				
				dict->Add("mathcalc", x, tempAns);

				if (dict->GetCorrectAnswer("mathcalc", x) == tempAns) {
					mathCorrect++;
				}
				else {
					v_MathCalcWrongAns.push_back(x + 1);
					(*v_TestAnsPatterns)[testQuestionNum]++;
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
				(*v_TestAnsPatterns)[testQuestionNum]++;

				continue;
			}

			bool correctAnswer = false;

			char buf[50];
			std::stringstream ss(dict->GetCorrectAnswer("mathcalc", x + 30));
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
				(*v_TestAnsPatterns)[testQuestionNum]++;
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
		for (unsigned int x = 0; x < (*v_TestAnsPatterns).size(); x++) {

			if ((*v_TestAnsPatterns)[x] > 1) {

				if (x < 53) { // Reading

					for (unsigned int z = 0; z < v_ReadingWrongAns.size(); z++) {
						
						if (x + 1 == v_ReadingWrongAns[z]) {
							std::cout << "Reading Question     [" << std::setfill('0') << std::setw(2) << x + 1 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << (*v_TestAnsPatterns)[x] << "] times." << std::endl;
						}
					}
				}
				else if (x < 97) { // Writing

					for (unsigned int z = 0; z < v_WritingWrongAns.size(); z++) {

						if (x - 51 == v_WritingWrongAns[z]) {
							std::cout << "Writing Question     [" << std::setfill('0') << std::setw(2) << x - 51 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << (*v_TestAnsPatterns)[x] << "] times." << std::endl;
						}
					}
				}
				else if (x < 117) { // Math - No Calculator

					for (unsigned int z = 0; z < v_MathNoCalcWrongAns.size(); z++) {

						if (x - 95 == v_MathNoCalcWrongAns[z]) {
							std::cout << "Math - No Calculator [" << std::setfill('0') << std::setw(2) << x - 95 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << (*v_TestAnsPatterns)[x] << "] times." << std::endl;
						}
					}
				}
				else { // Math - Calculator

					for (unsigned int z = 0; z < v_MathCalcWrongAns.size(); z++) {

						if (x - 115 == v_MathCalcWrongAns[z]) {
							std::cout << "Math - Calculator    [" << std::setfill('0') << std::setw(2) << x - 115 << "] was answered wrong ["
									  << std::setfill('0') << std::setw(2) << (*v_TestAnsPatterns)[x] << "] times." << std::endl;
						}
					}
				}
			}

			outAnsPatterns << (*v_TestAnsPatterns)[x] << std::endl;
		}
		outAnsPatterns.close();

		std::cout << std::endl;
		system("PAUSE");
		std::cout << std::endl;
		
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		delete dict;
		delete v_TestAnsPatterns;
	}

	return 0;
}
