#pragma once
#include <string>
using namespace std;
class cUserInputs
{
private:
	string m_strProcessName;
	bool m_bIsCreateNewProcess;
	bool m_bIsDefendProcess;
public:
	cUserInputs();
	~cUserInputs();

	void setProcessName(const string p_crefstrProcessName);
	string getProcessName() const;

	bool isCreateNewProcess()const;
	void setCreateProcess(const bool p_cbIsCreateProcess=true);

	bool isDefendProcess()const;
	void setDefendProcess(const bool p_cbIsDefendProcess = true);
};

