#pragma once
#include <vector>
#include "cUserInputs.h"
using namespace std;

class cCommandReader
{
private:
	const vector<string> &m_crefvtstrCommands;
	cUserInputs m_UserInputs;
	vector<string> m_vtstrErrorMessages;
	vector<string> m_vtstrWarningMessages;
public:
	cCommandReader(const vector<string> &p_crefvtstrCommands);
	~cCommandReader();
private:
	string getKey(const string &p_crefstrInputString) const;
	string getValue(const string &p_crefstrInputString) const;

private:
	void validateDllPath(const string &p_crefstrDllPath);
	void validateProcess(const string &p_crefstrDllPath);
	void validateFuncName(const string &p_crefstrFuncName);


public:
	void read();

public:
	cUserInputs getUserInputs()const;
	vector<string> getMessages()const;
	bool isErrorFound()const;
	void printHelp()const;
};

