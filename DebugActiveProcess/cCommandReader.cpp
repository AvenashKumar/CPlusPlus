#include "cCommandReader.h"
#include "sConstants.h"
#include "sStringUtils.h"
#include "sProcessUtils.h"
#include "sPathUtils.h"
cCommandReader::cCommandReader(const vector<string> &p_crefvtstrCommands) :
m_crefvtstrCommands(p_crefvtstrCommands)
{
}


cCommandReader::~cCommandReader()
{
}

string cCommandReader::getKey(const string &p_crefstrInputString) const
{
	size_t uiSpliterPos = p_crefstrInputString.find('=');
	if (uiSpliterPos != string::npos)
	{
		//No need to include "=" sign that's why use subtract one from the curr pos
		return p_crefstrInputString.substr(0, uiSpliterPos);
	}
	else
		return p_crefstrInputString;
}

string cCommandReader::getValue(const string &p_crefstrInputString) const
{
	size_t uiSpliterPos = p_crefstrInputString.find('=');

	vector<string> vtstrTokens;
	if (uiSpliterPos != string::npos)
	{
		//No need to include "=" sign, that add one in curr pos of "=" 
		return p_crefstrInputString.substr(uiSpliterPos + 1);
	}
	else
		return "";
}




void cCommandReader::validateProcess(const string &p_crefstrProcessName)
{
	if (sProcessUtils::isProcessExist(p_crefstrProcessName))
		m_UserInputs.setProcessName(p_crefstrProcessName);
	else
		addErrorMessage("Unable to find process - " + p_crefstrProcessName);
}


void cCommandReader::read()
{
	cUserInputs tempUserInputs;
	for (const string & strCommand : m_crefvtstrCommands)
	{
		string strKey = sStringUtils::toUpper(getKey(strCommand));
		string strVal = getValue(strCommand);

		if (strKey == sConstants::CMD_PROCESS)
		{
			tempUserInputs.setProcessName(strVal);
			continue;
		}
		if (strKey == sConstants::CMD_CREATE_PROCESS)
		{
			tempUserInputs.setCreateProcess();
			continue;
		}
		if (strKey == sConstants::CMD_DEFEND)
		{
			tempUserInputs.setDefendProcess();
			continue;
		}
	}
	validateProcess(tempUserInputs.getProcessName());
	m_UserInputs.setCreateProcess(tempUserInputs.isCreateNewProcess());
	m_UserInputs.setDefendProcess(tempUserInputs.isDefendProcess());
	printMessages();
}

void cCommandReader::printHelp()const
{
	cout << sConstants::CMD_PROCESS << "=Target process name" << endl;
	cout << sConstants::CMD_CREATE_PROCESS << ":= Create new process" << endl;
	cout << sConstants::CMD_DEFEND << ":= Defend process" << endl;
}

cUserInputs cCommandReader::getUserInputs()const
{
	return m_UserInputs;
}
