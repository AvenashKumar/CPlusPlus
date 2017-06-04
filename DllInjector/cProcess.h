#pragma once
#include <Windows.h>
#include <iostream>
#include "cHandle.h"
using namespace std;
class cProcess
{
private:
	DWORD m_uilProcessID;		///<Responsible for holding process id
	string m_strProcessName;	///<Responsible for holding process name
	BOOL m_bIs64BitProcess;		///<Indicate either process is 64bit or 32bit
	cHandle m_hProcessHandle;

public:
	cProcess(const DWORD p_cuiProcessID, const string &p_crefstrProcessName, const BOOL p_cbIs64bitProcess);

	cProcess();
	BOOL is64bitProcess()const;
	DWORD getProcessID()const;
	string getProcessName()const;
	cHandle& getProcessHandle();
};

