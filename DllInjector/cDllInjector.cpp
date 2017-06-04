#include <string>
#include "cProcess.h"
#include "cDllInjector.h"
#include "cHandle.h"
#include "cTokenGenerator.h"
cDllInjector::cDllInjector(cProcess &p_refTrgProcess,
	const string &p_crefstrDllName,
	PVOID *p_ptrpOutput) :
	m_pTrgProcess(p_refTrgProcess),
	m_hrefTrgProcessHandle(p_refTrgProcess.getProcessHandle()),
	m_cstrDllName(p_crefstrDllName),
	m_uilRemoteThreadState(0)
{
}

bool cDllInjector::openProcess()
{
	//Generate Access Token and "Debug" privilege
	//cTokenGenerator tokenGenerator(SE_DEBUG_NAME);
	//tokenGenerator.generate();
	//tokenGenerator.setPrivilege(TRUE);
	

	//All good
	SetLastError(ERROR_SUCCESS);

	//Open target process
	m_hrefTrgProcessHandle.setHandle(OpenProcess(PROCESS_ALL_ACCESS, TRUE, m_pTrgProcess.getProcessID()));
	if (m_hrefTrgProcessHandle.getHandle() == NULL)
	{
		//Since, we cannot attack any system process without admin privilages we have to, inform caller
		if (GetLastError() == ERROR_ACCESS_DENIED)
			return false;

		//Otherwise throw exception
		string strError = "Unable to Open process -> " +
			m_pTrgProcess.getProcessName() +
			", Error Code -> " + to_string(GetLastError());

		throw exception(strError.c_str());
	}

	//Unset "Debug" privilege
	//tokenGenerator.setPrivilege(FALSE);

	return true;
	//Unset "Debug" privilege
	//tokenGenerator.setPrivilege(FALSE);
}

LPVOID cDllInjector::getFunctionAddress(const string &p_crefstrDll, const string &p_crefstrExportedFunc)
{
	//In order to get address of function "LoadLibraryA" or functions which are part of the operating system dll like kernel32.dll 
	LPVOID lpFunctionAddress = GetProcAddress(GetModuleHandleA(p_crefstrDll.c_str()), p_crefstrExportedFunc.c_str());

	//In order to get address of normal functions
	if (!lpFunctionAddress)
		lpFunctionAddress = GetProcAddress(LoadLibraryA(p_crefstrDll.c_str()), p_crefstrExportedFunc.c_str());

	if (!lpFunctionAddress)
	{
		string strError = "Cannot get of function " + p_crefstrDll + "::" + p_crefstrExportedFunc + " address, error code: " + to_string(GetLastError());
		throw exception(strError.c_str());
	}

	return lpFunctionAddress;
}

LPVOID cDllInjector::allocateMemoryOfFuncParam(LPVOID p_lpParameters, SIZE_T p_uiParamSize)
{
	LPVOID lpBaseAddress = NULL;
	if (p_lpParameters)
	{
		lpBaseAddress = VirtualAllocEx(m_hrefTrgProcessHandle.getHandle(), NULL, p_uiParamSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (lpBaseAddress == NULL)
		{
			string strError = "Unable to allocate memmory in target process, error code: " + to_string(GetLastError());
			throw exception(strError.c_str());
		}

		SIZE_T uiBytesWritten = 0;
		BOOL bResult = WriteProcessMemory(m_hrefTrgProcessHandle.getHandle(),
			lpBaseAddress,
			p_lpParameters,
			p_uiParamSize,
			&uiBytesWritten);

		if (!bResult || uiBytesWritten < 1)
		{
			string strError = "Error unable to write memory, error code: " + to_string(GetLastError());
			throw exception(strError.c_str());
		}
	}
	return lpBaseAddress;
}

BOOL cDllInjector::isTargetProcessCompatibleWithDll()
{
	BOOL is64bitDll = FALSE;

#ifdef _M_X64
	is64bitDll = TRUE;
#endif

	return is64bitDll == m_pTrgProcess.is64bitProcess();
}

bool cDllInjector::injectDLL()
{
	if (!isTargetProcessCompatibleWithDll())
	{
		string strError = "Target process " + m_pTrgProcess.getProcessName() + " and input dll are in-compatible";
		throw exception(strError.c_str());
	}

	//Open process i.e. get process handle
	bool bIsProcessOpened = openProcess();

	if (!bIsProcessOpened)
		return false;

	//Get address of "LoadLibraryA" in order to inject input dll in target process
	LPVOID lpFunctionAddress = getFunctionAddress("kernel32.dll", "LoadLibraryA");

	//Parameter of "LoadLibraryA" is dll name/path.
	LPVOID lpBaseAddress = allocateMemoryOfFuncParam((LPVOID)m_cstrDllName.c_str(), m_cstrDllName.size());

	//Inject DLL
	cHandle hThreadHandle = CreateRemoteThread(m_hrefTrgProcessHandle.getHandle(), NULL, 0,
		(LPTHREAD_START_ROUTINE)lpFunctionAddress, lpBaseAddress, NULL, NULL);

	if (!hThreadHandle.getHandle())
	{
		string strError = "Unable to inject dll in target process, error code: " + to_string(GetLastError());
		throw exception(strError.c_str());
	}

	//Wait until created thread is "STILL_ACTIVE".
	while (GetExitCodeThread(hThreadHandle.getHandle(), &m_uilRemoteThreadState))
	{
		if (m_uilRemoteThreadState != STILL_ACTIVE)
		{
			break;
		}
	}

	return true;
}

void cDllInjector::executeFunction(const string &p_crefstrExportedFunction, LPVOID p_lpParameters, SIZE_T p_uiParamSize)
{
	if (m_hrefTrgProcessHandle.getHandle() == NULL)
	{
		string strError = "DLL " + m_cstrDllName + " is un-injected.";
		throw exception(strError.c_str());
	}

	//Get address of exported function
	LPVOID lpFunctionAddress = getFunctionAddress(m_cstrDllName, p_crefstrExportedFunction);

	//Allocate memory for function parameters.
	LPVOID lpBaseAddress = allocateMemoryOfFuncParam(p_lpParameters, p_uiParamSize);

	//Execute function via remote thread
	HANDLE hThreadHandle = CreateRemoteThread(m_hrefTrgProcessHandle.getHandle(), NULL, 0,
		(LPTHREAD_START_ROUTINE)lpFunctionAddress, lpBaseAddress, NULL, NULL);

	if (!hThreadHandle)
	{
		string strError = "Unable to inject dll in target process, error code: " + to_string(GetLastError());
		throw exception(strError.c_str());
	}

	//Wait until created thread is "STILL_ACTIVE" (no need to wait in RELEASE).
#ifdef _DEBUG
	while (GetExitCodeThread(hThreadHandle, &m_uilRemoteThreadState))
	{
		if (m_uilRemoteThreadState != STILL_ACTIVE)
		{
			break;
		}
	}
#endif
}
