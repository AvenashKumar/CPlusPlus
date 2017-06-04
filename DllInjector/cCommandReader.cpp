#include "cCommandReader.h"
#include "sConstants.h"
#include "sStringUtils.h"
#include "sFileHelper.h"
#include "sProcessUtils.h"
#include "sPathUtils.h"
cCommandReader::cCommandReader(const vector<string> &p_crefvtstrCommands):
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
		return p_crefstrInputString.substr(uiSpliterPos+1);
	}
	else
		return "";
}

void cCommandReader::validateDllPath(const string &p_crefstrDllPath)
{
	string strApplicationDir = "";
	sPathUtils::getCurrExecutableDirPath(strApplicationDir);
	string strDllPath = strApplicationDir + R"(\)" + p_crefstrDllPath;

	//Check dll exist: Path where application run
	if (sFileHelper::isFileExist(strDllPath))
	{
		m_UserInputs.setDllPath(strDllPath);
	}
	//Check dll exist at provided path
	else if (sFileHelper::isFileExist(p_crefstrDllPath))
		m_UserInputs.setDllPath(p_crefstrDllPath);
	//Error
	else
		m_vtstrErrorMessages.push_back(sConstants::STR_ERROR + "DLL path invalid - " + p_crefstrDllPath);
}

void cCommandReader::validateFuncName(const string &p_crefstrFuncName)
{
	if (p_crefstrFuncName.empty())
		m_vtstrErrorMessages.push_back(sConstants::STR_ERROR + " Function name is empty");
	else
		m_UserInputs.setFuncName(p_crefstrFuncName);
}

void cCommandReader::validateProcess(const string &p_crefstrProcessName)
{
	if (sProcessUtils::isProcessExist(p_crefstrProcessName))
		m_UserInputs.setProcessName(p_crefstrProcessName);
	else
		m_vtstrErrorMessages.push_back(sConstants::STR_ERROR + "Unable to find process - " + p_crefstrProcessName);
}


void cCommandReader::read()
{
	if (m_crefvtstrCommands.size() < 2)
	{
		printHelp();
		throw exception("Invalid commands/arguments");
	}

	cUserInputs tempUserInputs;
	for (const string & strCommand : m_crefvtstrCommands)
	{
		string strKey = sStringUtils::toUpper(getKey(strCommand));
		string strVal = getValue(strCommand);
		if (strKey == sConstants::CMD_DLL)
		{
			tempUserInputs.setDllPath(strVal);
			continue;
		}
		if (strKey == sConstants::CMD_PROCESS)
		{
			tempUserInputs.setProcessName(strVal);
			continue;
		}
		if (strKey == sConstants::CMD_FUNC)
		{
			tempUserInputs.setFuncName(strVal);
			continue;
		}
	}
	validateDllPath(tempUserInputs.getDllPath());
	validateProcess(tempUserInputs.getProcessName());
	validateFuncName(tempUserInputs.getFuncName());
}

bool cCommandReader::isErrorFound()const
{
	return m_vtstrErrorMessages.size() > 0;
}

void cCommandReader::printHelp()const
{
	cout << sConstants::CMD_DLL<<"=Dll Path" << endl;
	cout << sConstants::CMD_PROCESS<<"=Target process name" << endl;
	cout << sConstants::CMD_FUNC << "=Function name" << endl;
}

vector<string> cCommandReader::getMessages()const
{
	vector<string> vtstrMessages;
	vtstrMessages.reserve(m_vtstrWarningMessages.size() + m_vtstrErrorMessages.size());
	vtstrMessages.insert(vtstrMessages.end(), m_vtstrWarningMessages.begin(), m_vtstrWarningMessages.end());
	vtstrMessages.insert(vtstrMessages.end(), m_vtstrErrorMessages.begin(), m_vtstrErrorMessages.end());
	return vtstrMessages;
}
cUserInputs cCommandReader::getUserInputs()const
{
	return m_UserInputs;
}
