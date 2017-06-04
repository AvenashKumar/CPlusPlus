#pragma once
#include <vector>
using namespace std;
class cMessages
{
private:
	vector<string> m_vtstrErrorMsgs;
	vector<string> m_vtstrWarningMsgs;
public:
	cMessages();
	~cMessages();

public:
	void addErrorMessage(const string &p_crefstrErrorMsgs);
	void addWarningMessage(const string &p_crefstrWarningMsgs);

public:
	void getMessages(vector<string>&p_refvtstrMessages)const;
	bool isErrorFound()const;
	void printMessages()const;
};

