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

/* Class: FileAutoGen
 * Author: Andrew Woo
 *
 * Purpose:
 *	This class generates the minimum necessary files for grading and discovering
 *	patterns in the 8 official College Board SAT Practice Tests, including
 *	all .ans, .pattern, .rawscore files.
 *
 * Warning:
 *	This function is CRITICAL for proper functionality of SATpatt. DO NOT EDIT
 *	WITHOUT PROPER KNOWLEDGE OF ITS FUNCTIONALITY. EDITING OF FUNCTIONS WITHIN
 *	COULD LEAD TO CRITICAL/FATAL ERRORS.
**/

#include "fileautogen.h"

#include <iostream>
#include <fstream>
#include <sstream>


// Splits string given delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {

	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);

	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

FileAutoGen::FileAutoGen() {}
FileAutoGen::~FileAutoGen() {}

// Initial setup (first time run only, or when files are corrupted).
void FileAutoGen::initSetup() {

	std::ifstream in_initSetup("init.txt");
	if (!in_initSetup.is_open()) {

		std::ofstream out_initSetup("init.txt");

		if (generate()) {
			out_initSetup << "init_completed ::: true";
		}
		else {
			out_initSetup << "init_completed ::: false";
		}
		
		out_initSetup.close();
	}
	else {

		std::string temp = "";
		getline(in_initSetup, temp);

		if (temp == "init_completed ::: false") {
			
			std::ofstream out_initSetup("init.txt");

			if (generate()) {
				out_initSetup << "init_completed ::: true";
			}
			else {
				out_initSetup << "init_completed ::: false";
			}

			out_initSetup.close();
		}
	}
}

bool FileAutoGen::generate() {

	/// <summary>
	/// NECESSARY STRINGS FOR FILE GEN. DO NOT EDIT.
	/// </summary>

	reading_rawscore.push_back("10 10 10 11 12 13 14 15 15 16 17 17 18 19 19 20 20 21 21 22 22 23 23 24 24 25 25 26 26 27 28 28 29 29 30 30 31 31 32 32 33 33 34 35 35 36 37 37 38 38 39 40 40");
	reading_rawscore.push_back("10 10 10 11 12 13 14 15 15 16 17 18 18 19 19 20 20 21 21 22 22 23 23 24 24 24 25 25 26 27 27 28 28 29 29 30 30 31 31 32 32 33 33 34 34 35 35 36 37 37 38 39 40");
	reading_rawscore.push_back("10 10 10 11 12 13 14 14 15 16 17 17 18 19 19 20 21 21 22 22 23 24 24 25 25 26 26 27 27 28 28 29 29 29 30 30 31 31 32 32 33 33 34 34 35 36 36 37 38 38 39 40 40");
	reading_rawscore.push_back("10 10 10 11 12 13 14 15 15 16 17 18 18 19 19 20 20 21 21 22 22 23 23 24 24 24 25 25 26 27 27 28 28 29 29 30 30 31 31 32 32 33 33 34 34 35 35 36 37 37 38 39 40");
	reading_rawscore.push_back("10 10 10 10 11 11 12 13 14 14 14 16 16 17 17 18 18 19 20 20 21 21 22 23 23 24 24 25 25 26 26 27 28 28 29 29 29 30 30 31 31 32 32 33 33 34 35 35 36 37 37 39 40");
	reading_rawscore.push_back("10 10 10 10 11 12 13 14 15 15 16 17 17 18 18 19 19 20 20 21 21 22 22 23 23 24 24 25 25 26 26 27 27 28 28 29 29 30 30 31 31 32 33 33 34 35 36 37 37 38 39 40 40");
	reading_rawscore.push_back("10 10 10 10 11 12 13 14 15 15 16 17 17 18 18 19 20 20 21 21 22 22 23 23 24 24 25 25 26 26 27 27 28 28 29 29 30 30 31 31 32 32 33 34 35 35 36 37 37 38 39 39 40");
	reading_rawscore.push_back("10 10 10 10 11 12 13 14 15 15 16 17 17 18 18 19 19 20 20 21 21 22 22 23 23 24 24 25 25 26 26 27 27 28 28 29 29 29 30 30 31 31 32 33 33 34 35 35 36 37 38 39 40");

	reading_ans.push_back("BBCACDDBCBABDAACCDABABDDCBDCAADBACBDCCBCBBAADCBADBDA");
	reading_ans.push_back("ABCADBDDBDDDABCACCABCCABCBDDDBCBBADBBDCABDCBDCABDDDA");
	reading_ans.push_back("BCAACAABBDACDBBCBBAADAABCCBBDDBCCDCADCADACCDDCBBABDD");
	reading_ans.push_back("CDDCAABDDACDABACCABADACCBCABBDDDDADBDDDABCBADADCDCBA");
	reading_ans.push_back("DCCACADBBBBABDCABBBADABABCDBDBDBCBABCABDDCDDCBACCAAB");
	reading_ans.push_back("CBDACDBBADBDCCBADAACCBADCADAABBDBADDADCCBDCACDBBDBDB");
	reading_ans.push_back("DAABDBCBDDDAADDCDBDCBCADCACBCDBDBCBBCACAACDDCACAABAD");
	reading_ans.push_back("ACCDADDBCBBDDADBCBCACADABDBADCDBCBCBCDCADACACADBBBCC");

	writing_rawscore.push_back("10 10 10 10 11 12 13 13 14 15 16 16 17 18 19 19 20 21 21 22 23 23 24 25 25 26 26 27 28 28 29 30 30 31 32 32 33 34 34 35 36 37 38 39 40");
	writing_rawscore.push_back("10 10 10 11 12 13 14 14 15 16 17 17 18 18 19 20 20 21 22 22 23 23 24 25 25 26 26 27 27 28 29 29 30 31 31 32 33 33 34 35 36 37 38 39 40");
	writing_rawscore.push_back("10 10 10 11 12 13 14 15 15 16 17 18 19 19 20 21 22 22 23 24 24 25 25 26 26 27 27 28 29 29 30 30 31 32 33 33 34 34 35 35 36 37 38 39 40");
	writing_rawscore.push_back("10 10 10 11 12 13 14 14 15 16 17 17 18 18 19 20 20 21 22 22 23 23 24 25 25 26 26 27 27 28 29 29 30 31 31 32 33 33 34 35 36 37 38 39 40");
	writing_rawscore.push_back("10 10 10 10 11 12 13 13 14 15 16 16 17 18 18 19 20 20 21 22 23 23 24 25 25 26 27 28 28 29 30 30 31 32 32 33 34 34 35 36 37 38 39 40 40");
	writing_rawscore.push_back("10 10 10 11 11 12 13 14 15 16 16 17 18 18 19 20 20 21 22 23 23 24 25 25 26 27 27 28 28 29 30 30 31 31 32 33 34 34 35 36 36 38 39 39 40");
	writing_rawscore.push_back("10 10 10 10 11 12 12 13 14 15 15 16 17 18 18 19 19 20 21 22 22 23 24 25 25 26 26 27 28 29 29 30 31 31 32 32 33 34 34 35 36 36 37 39 40");
	writing_rawscore.push_back("10 10 10 10 11 12 13 14 14 15 16 17 17 18 19 19 20 21 21 22 22 23 24 24 25 25 26 27 27 28 29 29 30 31 31 32 33 33 34 35 36 37 38 39 40");

	writing_ans.push_back("DBACCDBCAABBABCCCADDBDDDBABCBDCAAAABDCABBCDD");
	writing_ans.push_back("BBAADDBDBBCBDCCCBBADDBABBADACCDBDDADABCDDCCD");
	writing_ans.push_back("ABCCABADCCBACDBCCBDCDAADBADBBBDBCDBCDCCBDADD");
	writing_ans.push_back("BBBADBDBCACDBDCCACACBDCCBDCADBCBACCBDACBDDAB");
	writing_ans.push_back("CDBCACDDBCCDDABCCADDABBBABDACBCDACBBDCDBDAAC");
	writing_ans.push_back("DADBCBACDBCDABCBACDDAABABBCDBCACDBBDADDCADAC");
	writing_ans.push_back("DABACCBDADADCACDBCBDCCCACDBDCBCCCABCDCDBADCA");
	writing_ans.push_back("DBCBDCBCACAADCCADBDBBDACCACCBBBDDBDABDBDADAC");

	math_rawscore.push_back("200 200 210 230 240 260 280 290 310 320 330 340 360 370 380 390 410 420 430 440 450 460 470 480 480 490 500 510 520 520 530 540 550 560 560 570 580 590 600 600 610 620 630 640 650 660 670 670 680 690 700 710 730 740 750 760 780 790 800");
	math_rawscore.push_back("200 200 210 230 250 270 290 300 320 330 340 360 370 380 390 400 420 430 440 450 460 470 480 490 500 510 510 520 530 540 550 560 570 570 580 590 600 610 620 630 640 650 650 660 670 680 690 690 700 710 720 730 740 750 760 770 780 790 800");
	math_rawscore.push_back("200 200 210 230 250 270 290 300 320 330 340 360 370 380 390 410 420 430 440 450 460 470 480 490 500 510 530 540 550 560 570 580 580 590 600 610 620 630 630 640 650 660 660 670 680 680 690 690 700 710 710 720 730 740 750 770 780 790 800");
	math_rawscore.push_back("200 200 210 230 250 270 290 300 320 330 340 360 370 380 390 400 420 430 440 450 460 470 480 490 500 510 510 520 530 540 550 560 570 570 580 590 600 610 620 630 640 650 650 660 670 680 690 690 700 710 720 730 740 750 760 770 780 790 800");
	math_rawscore.push_back("200 200 210 230 250 260 270 290 300 320 330 340 360 370 390 400 410 420 430 440 450 460 470 480 490 500 510 510 520 530 540 540 550 560 570 580 590 600 600 610 620 630 640 650 660 660 670 680 690 700 710 710 720 730 750 760 770 790 800");
	math_rawscore.push_back("200 200 210 230 250 260 280 290 310 320 330 340 350 360 380 390 400 410 420 430 440 450 460 470 490 500 510 510 520 530 530 540 550 560 570 580 590 590 600 610 620 630 640 650 660 670 670 680 690 700 710 720 730 740 760 770 780 790 800");
	math_rawscore.push_back("200 200 210 230 250 260 280 290 310 320 330 350 360 370 380 390 400 420 430 430 440 450 460 470 480 490 500 510 510 520 530 530 540 550 550 560 570 580 590 590 600 610 620 630 640 650 660 670 680 680 690 700 720 730 740 760 770 790 800");
	math_rawscore.push_back("200 210 220 240 250 270 290 300 320 340 350 360 380 390 400 420 430 440 450 460 470 480 490 500 510 520 520 530 540 540 550 560 570 580 580 590 600 610 620 630 640 650 660 670 680 680 690 700 710 730 740 750 770 780 790 790 800 800 800");

	mathnocalc_ans.push_back("DACBCABCBADDBAD,2,1600,7,4/5_0.8,100");
	mathnocalc_ans.push_back("CBAACDACBCCBDAD,3_6_9,19,12,6,1/4_0.25");
	mathnocalc_ans.push_back("CDDBCCCAAABABAD,1_2,2,105,370,3/5_0.6");
	mathnocalc_ans.push_back("AAABCBDADDCCBAB,9,3/5_0.6,5,0,25");
	mathnocalc_ans.push_back("DABCDACAABCDBCD,4,6/5_1.2,21/4_5.25,2,97");
	mathnocalc_ans.push_back("BBCADACBCDBDAAD,1_2_4_8_16,15/4_3.75,30,3/2_1.5,1/6_0.166_0.167");
	mathnocalc_ans.push_back("CCBDACBCBBDBDAD,8,30,4,8,6632");
	mathnocalc_ans.push_back("DAACBBBDACBDCCD,3,32,1.5_3/2,8,144");

	mathcalc_ans.push_back("BCDCDDCDABACCCACBABDCBBADBCCDD,4_5_6,107,5/8_0.625,96,6,3,1.02,6.11");
	mathcalc_ans.push_back("CBACCBDDABBDDCABCCBCDBAAADDBBA,14,7,11,105,15,32,3284,7500");
	mathcalc_ans.push_back("CBCCBADCBDBDDAABBBCBCBCDDCCDAA,4_5,58.6,9,5/8_0.625,50,750,7,60");
	mathcalc_ans.push_back("BCCBBAADBAACCDBADCACCBBCBCDDBD,1160,1/2_0.5,4.55,150,9/4_2.25,29_30_31_32_33_34,0.72,134");
	mathcalc_ans.push_back("DCABCBACBAADDAADDCBDACDBDBCCBB,1492,29/3_9.66_9.67,7,9,13,80,43_43.5_44_87/2,6");
	mathcalc_ans.push_back("ACADBCDDBBBDABDBBCCCDBCDBCAABD,10,31,97_98_99_100_101,5,1.25_5/4,2.6_13/5,30,8");
	mathcalc_ans.push_back("BACDCCACADABBDBDABACCBCABBACDB,195,2/5_0.4,30,5/18_0.277_0.278,0_3,6,2.4_12/5,5/7_0.714");
	mathcalc_ans.push_back("ACACBDCBDCBCCDDBACABDAADACBDBB,102,2,30,25.4_127/5,2_8,0,576,0.8_4/5");

	/// <summary>
	/// NECESSARY STRINGS FOR FILE GEN. DO NOT EDIT.
	/// </summary>

	std::ofstream _ans;
	std::ofstream _rawscore;
	std::ofstream _pattern;

	for (int x = 0; x < 8; x++) {

		std::string temp = "";

		/// Generate .pattern Files
		temp = "ans\\0" + std::to_string(x + 1) + "_test.pattern";
		_pattern.open(temp);
		for (int x = 0; x < 154; x++) {
			_pattern << 0 << std::endl;
		}
		_pattern.close();
		/// Generate .pattern Files

		char buf[52];

		/// Generate Files for Reading Section
		std::stringstream r_ss(reading_rawscore[x]);

		temp = "ans\\0" + std::to_string(x + 1) + "_reading.rawscore";
		_rawscore.open(temp);
		while (r_ss >> buf) {
			_rawscore << buf << std::endl;
		}
		_rawscore.close();

		temp = "ans\\0" + std::to_string(x + 1) + "_reading.ans";
		_ans.open(temp);
		for (unsigned int z = 0; z < reading_ans[x].size(); z++) {
			_ans << reading_ans[x][z] << std::endl;
		}
		_ans.close();
		/// Generate Files for Reading Section

		/// Generate Files for Writing Section
		std::stringstream w_ss(writing_rawscore[x]);

		temp = "ans\\0" + std::to_string(x + 1) + "_writing.rawscore";
		_rawscore.open(temp);
		while (w_ss >> buf) {
			_rawscore << buf << std::endl;
		}
		_rawscore.close();

		temp = "ans\\0" + std::to_string(x + 1) + "_writing.ans";
		_ans.open(temp);
		for (unsigned int z = 0; z < writing_ans[x].size(); z++) {
			_ans << writing_ans[x][z] << std::endl;
		}
		_ans.close();
		/// Generate Files for Writing Section

		/// Generate Files for Math - No Calculator & Math - Calculator
		std::stringstream m_ss(math_rawscore[x]);

		temp = "ans\\0" + std::to_string(x + 1) + "_math.rawscore";
		_rawscore.open(temp);
		while (m_ss >> buf) {
			_rawscore << buf << std::endl;
		}
		_rawscore.close();

		temp = "ans\\0" + std::to_string(x + 1) + "_mathnocalc.ans";
		_ans.open(temp);

		std::vector<std::string> v_split_mathnocalc = split(mathnocalc_ans[x], 44);
		for (unsigned int s = 0; s < v_split_mathnocalc[0].size(); s++) {
			_ans << v_split_mathnocalc[0][s] << std::endl;
		}

		for (unsigned int s = 1; s < v_split_mathnocalc.size(); s++) {

			std::vector<std::string> v_split_mathnocalc_s = split(v_split_mathnocalc[s], 95);
			for (unsigned int r = 0; r < v_split_mathnocalc_s.size(); r++) {
				_ans << v_split_mathnocalc_s[r] << " ";
			}
			_ans << std::endl;
		}

		_ans.close();

		temp = "ans\\0" + std::to_string(x + 1) + "_mathcalc.ans";
		_ans.open(temp);

		std::vector<std::string> v_split_mathcalc = split(mathcalc_ans[x], 44);
		for (unsigned int s = 0; s < v_split_mathcalc[0].size(); s++) {
			_ans << v_split_mathcalc[0][s] << std::endl;
		}

		for (unsigned int s = 1; s < v_split_mathcalc.size(); s++) {

			std::vector<std::string> v_split_mathcalc_s = split(v_split_mathcalc[s], 95);
			for (unsigned int r = 0; r < v_split_mathcalc_s.size(); r++) {
				_ans << v_split_mathcalc_s[r] << " ";
			}
			_ans << std::endl;
		}

		_ans.close();
		/// Generate Files for Math - No Calculator & Math - Calculator
	}

	return true;
}
