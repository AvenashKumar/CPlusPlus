#pragma once
#include <Windows.h>
#include <string>
#include "cProcess.h"
#include "cHandle.h"
class cDllInjector
{
private:
	cProcess m_pTrgProcess;
	cHandle &m_hrefTrgProcessHandle;
	const string m_cstrDllName;
	DWORD m_uilRemoteThreadState;
public:
	cDllInjector(cProcess &p_crefTrgProcess,
		const string &p_crefstrDllName,
		PVOID *p_ptrpOutput);

	bool injectDLL();
	void executeFunction(const string &p_crefstrExportedFunction, LPVOID p_lpParameters, SIZE_T p_uiParamSize);
private:
	/** Responsible for returning function address that resides in dll
	* param p_crefstrDll: Dll path
	* param p_crefstrExportedFunc: Exported function
	* return function address
	*/
	LPVOID getFunctionAddress(const string &p_crefstrDll, const string &p_crefstrExportedFunc);
	LPVOID allocateMemoryOfFuncParam(LPVOID p_lpParameters, SIZE_T p_uiParamSize);

	bool openProcess();

	/** Indicate: Either target process is compatible with input dll or not,
	Compatible means, a 64bit dll is compatible with 64bit process, viceversa.
	* return True when dll and target process are compatible, otherwise returns False
	*/
	BOOL isTargetProcessCompatibleWithDll();
};

