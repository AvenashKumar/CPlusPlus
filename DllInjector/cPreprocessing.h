#pragma once
#include <vector>
using namespace std;
class cUserInputs;
class cPreprocessing
{
private:
	const vector<string> &m_crefvtstrArgs;
	cUserInputs &m_refUserInputs;
public:
	cPreprocessing(const vector<string> &p_crefvtstrArgs, cUserInputs &p_refUserInputs);
	~cPreprocessing();

public:
	void process();

private:
	void readCommands();
	void printErrorMessages(const vector<string> &p_crefvtstrMessages);
};

