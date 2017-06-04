#pragma once
#include <vector>
using namespace std;
class sStringUtils
{
public:
	static void tokenize(const string &p_crefstrInputString, vector<string> &p_refvtTokens);
	static string trim(const string& p_crefstrInput);
	static string toUpper(const string &p_refstrInputString);
};

