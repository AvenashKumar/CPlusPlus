#pragma once
#include <vector>
#include "cUserInputs.h"
#include "cMessages.h"
using namespace std;

class cCommandReader : public cMessages
{
private:
	const vector<string> &m_crefvtstrCommands;
	cUserInputs m_UserInputs;
public:
	cCommandReader(const vector<string> &p_crefvtstrCommands);
	~cCommandReader();

private:
	string getKey(const string &p_crefstrInputString) const;
	string getValue(const string &p_crefstrInputString) const;

private:
	void validateProcess(const string &p_crefstrDllPath);

public:
	void read();

public:
	cUserInputs getUserInputs()const;
	void printHelp()const;
};

