#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <regex>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;

int min_edit(string fixed_line, string change_line);
int lev_edit(string fixed_line, string change_line);
int find_min(int a, int b, int c);
int find_max(int a, int b);
void PD_method1(vector<string> s, vector<string> t);
void PD_method2(vector<string> s, vector<string> t);
float find_max_vector(vector<float> d);
static inline void rtrim(string& s);
static inline void ltrim(string& s);
static inline void trim(string& s);
float standard_error(vector<float> d);

int main() {
	while (true) {
		string fixed_line;
		string change_line;

		string question;
		cout << "1) Minimum & Lecenshtein Distance";
		cout << endl << "2) Plagiarism Detection";
		cout << endl << "Enter which question you want to see > ";
		getline(cin, question);
		if (question == "1") {
			cout << endl << endl << "Enter the two lines > " << endl;
			getline(cin, fixed_line);
			getline(cin, change_line);

			cout << "The minimum edit distance is " << min_edit(fixed_line, change_line) << endl;
			cout << "The Levenshtein distance is " << lev_edit(fixed_line, change_line) << endl << endl << endl;
		}
		else if (question == "2") {
			fstream FILE;
			string s_filename;
			cout << endl << endl;
			cout << "T file num > ";
			string T_num_s;
			getline(cin, T_num_s);
			int T_num = stoi(T_num_s);
			cout << "Enter S filename > ";
			getline(cin, s_filename);
			FILE.open(s_filename, ios::in);
			vector<string> context1;
			vector<string> context2;
			string s;
			while (getline(FILE, s)) {
				context1.push_back(s);
			}
			FILE.close();
			string T_filename;
			for (int i = 0; i < T_num; i++) {
				cout << "Enter T filename > ";
				getline(cin, T_filename);
				FILE.open(T_filename, ios::in);
				while (getline(FILE, s)) {
					context2.push_back(s);
				}
				FILE.close();

				string method;
				cout << endl << "1) method 1" << endl;
				cout << "2) method 2" << endl;
				cout << "Select method 1 or 2 > ";
				getline(cin, method);
				if (method == "1") {
					PD_method1(context1, context2);
				}
				else if (method == "2") {
					PD_method2(context1, context2);
				}
				else {
					cout << "WRONG!!!" << endl << endl;
				}
			}
		}
		else {
			cout << endl << "Enter 1 or 2" << endl << endl;
		}
	}
}

int min_edit(string fixed_line, string change_line) {
	vector<vector<int>> array;
	vector<int> array_store;
	for (int i = 0; i < fixed_line.length()+1; i++) array_store.push_back(i);
	array.push_back(array_store);
	for (int i = 1; i < change_line.length()+1; i++) {
		array_store.clear();
		array_store.push_back(i);
		array.push_back(array_store);
	}
	for (int i = 1; i < array.size(); i++) {
		for (int j = 1; j < array[0].size(); j++) {
			int sub = 0;
			if (fixed_line[j - 1] == change_line[i - 1]) sub = array[i - 1][j - 1];
			else sub = array[i - 1][j - 1] + 2;
			array[i].push_back(find_min(array[i - 1][j] + 1, array[i][j - 1] + 1, sub));
		}
	}
	return array[change_line.length() - 1][fixed_line.length() - 1];
}

int lev_edit(string fixed_line, string change_line) {
	vector<vector<int>> array;
	vector<int> array_store;
	for (int i = 0; i < fixed_line.length() + 1; i++) array_store.push_back(i);
	array.push_back(array_store);
	for (int i = 1; i < change_line.length() + 1; i++) {
		array_store.clear();
		array_store.push_back(i);
		array.push_back(array_store);
	}
	for (int i = 1; i < array.size(); i++) {
		for (int j = 1; j < array[0].size(); j++) {
			int sub = 0;
			if (fixed_line[j - 1] == change_line[i - 1]) sub = array[i - 1][j - 1];
			else sub = array[i - 1][j - 1] + 1;
			array[i].push_back(find_min(array[i - 1][j] + 1, array[i][j - 1] + 1, sub));
		}
	}
	return array[change_line.length() - 1][fixed_line.length() - 1];
}

int find_min(int a, int b, int c) {
	int min;
	min = a;
	if (min > b) min = b;
	if (min > c) min = c;
	return min;
}

void PD_method1(vector<string> s, vector<string> t) {
	int total_s = 0;
	int total_t = 0;
	int minED = 0;
	for (int i = 0; i < s.size(); i++) {
		regex reg(R"(\s+)");
		s[i] = regex_replace(s[i], reg, " ");
		trim(s[i]);
		if ((s[i].length() == 1 && s[i][0] == ' ') || s[i].length() == 0) {
			s.erase(s.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < t.size(); i++) {
		regex reg(R"(\s+)");
		t[i] = regex_replace(t[i], reg, " ");
		trim(t[i]);
		if ((t[i].length() == 1 && t[i][0] == ' ') || t[i].length() == 0) {
			t.erase(t.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < s.size(); i++) total_s += s[i].length();
	for (int i = 0; i < t.size(); i++) total_t += t[i].length();
	int max = find_max(total_s, total_t);
	for (int i = 0; i < s.size(); i++) minED += min_edit(s[i], t[i]);
	float result = 1 - (float)minED / (float)max;
	cout << endl << "d(s,t) = " << result << endl << endl;
	//for (int i = 0; i < t.size(); i++) cout << t[i] << endl;
}

int find_max(int a, int b) {
	if (a > b) return a;
	return b;
}

void PD_method2(vector<string> s, vector<string> t) {
	for (int i = 0; i < s.size(); i++) {
		regex reg(R"(\s+)");
		s[i] = regex_replace(s[i], reg, " ");
		trim(s[i]);
	}
	for (int i = 0; i < t.size(); i++) {
		regex reg(R"(\s+)");
		t[i] = regex_replace(t[i], reg, " ");
		trim(t[i]);
	}
	vector<string> context1;
	vector<string> context2;
	int count = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i].length() != 0 && i < s.size()) {
			context1.push_back(s[i]);
			i++;
		}
		if (i >= s.size()) break;
		while (s[i].length() != 0 && i < s.size() - 1 && !context1.empty()) {
			context1[count].append(s[i]);
			i++;
		}
		if (s[i].length() == 0 && !context1.empty()) count++;
	}
	count = 0;
	for (int i = 0; i < t.size(); i++) {
		if (t[i].length() != 0 && i < t.size()) {
			context2.push_back(t[i]);
			i++;
		}
		if (i >= t.size()) break;
		while (t[i].length() != 0 && i < t.size() - 1 && !context2.empty()) {
			context2[count].append(t[i]);
			i++;
		}
		if (t[i].length() == 0 && !context2.empty()) count++;
	}
	vector<float> d;
	vector<float> dmax;
	for (int i = 0; i < context1.size(); i++) {
		d.clear();
		for (int j = 0; j < context2.size(); j++) {
			float min = (float)min_edit(context1[i], context2[j]);
			float max = (float)find_max(context1[i].length(), context2[j].length());
			float store = 1 - min / max;
			if (store < 0) d.push_back(0);
			else d.push_back(store);
		}
		dmax.push_back(find_max_vector(d));
	}
	float d_sum = 0;
	for (int i = 0; i < dmax.size(); i++) d_sum += dmax[i];
	float d_st = d_sum / dmax.size();
	cout << endl << endl << "d(s,t) = " << d_st << endl;
	cout << "Variance = " << pow(standard_error(dmax), 2) << endl;
	cout << "Standart Error = " << standard_error(dmax) << endl << endl << endl;
}

float find_max_vector(vector<float> d) {
	float max = d[0];
	for (int i = 1; i < d.size(); i++) {
		if (max < d[i]) max = d[i];
	}
	return max;
}

static inline void ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
	}));
}

static inline void rtrim(string& s) {
	s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !isspace(ch);
	}).base(), s.end());
}

static inline void trim(string& s) {
	rtrim(s);
	ltrim(s);
}

float standard_error(vector<float> d) {
	float avg;
	float sum = 0;
	for (int i = 0; i < d.size(); i++) sum += d[i];
	avg = sum / (float)d.size();
	float sum_cal = 0;
	for (int i = 0; i < d.size(); i++) sum_cal += (float)pow(d[i] - avg, 2);
	return (float)sqrt(sum_cal / (float)d.size());
}