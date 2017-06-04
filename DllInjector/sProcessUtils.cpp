#include "sProcessUtils.h"
#include "sPathUtils.h"
#include "cHandle.h"
#include <Windows.h>
#include <string>
#include <wtsapi32.h>
#pragma comment ( lib, "WtsApi32.lib" )


bool sProcessUtils::isProcessExist(const string &p_crefvtstrProcessName)
{
	vector<cProcess> vtProcess;
	getAllProcess(vtProcess);
	for (const cProcess &process : vtProcess)
	{
		if (process.getProcessName() == p_crefvtstrProcessName)
			return true;
	}
	return false;
}

BOOL sProcessUtils::isProcess64bit(const DWORD p_uilProcessID)
{
	string strError = "Unable to Find -> Process interopability, Function -> ";

	BOOL bIs64bitProcess = FALSE;

	//Get process handle
	cHandle hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, p_uilProcessID);
	if (hProcess.getHandle() != NULL)
	{

		/*For compatibility with operating systems that do not support this function,
		call GetProcAddress to detect whether IsWow64Process is implemented in Kernel32.dll.
		If GetProcAddress succeeds, it is safe to call this function. Otherwise, WOW64 is not present.*/
		typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
		LPFN_ISWOW64PROCESS fnIsWow64Process;
		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

		//bIs64bitProcess:=true => 32bit, otherwise 64bit
		if (fnIsWow64Process(hProcess.getHandle(), &bIs64bitProcess))
		{
			return !bIs64bitProcess;
		}
		else
		{
			string strErrorWithErrCode = strError + "GetProcAddress -> Error Code -> " + to_string(GetLastError());
			throw exception(strErrorWithErrCode.c_str());
		}
	}

	return bIs64bitProcess;
}

cProcess sProcessUtils::getProcess(const string &p_refstrProcessName)
{
	vector<cProcess> vtProcess;
	getAllProcess(vtProcess);
	for (cProcess process : vtProcess)
	{
		if (process.getProcessName() == p_refstrProcessName)
		{
			return process;
		}
	}
	string strError = "Process: " + p_refstrProcessName + " not found";
	throw exception(strError.c_str());
}

void sProcessUtils::getAllProcess(vector<cProcess> & p_refvtProcess)
{
	char procs[4096];
	ZeroMemory(&procs, sizeof(procs));
	PWTS_PROCESS_INFO pProcessInfo;
	DWORD ProcessCount = 0;
	string strApplicationName = "";
	sPathUtils::getApplicationName(strApplicationName);
	if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pProcessInfo, &ProcessCount))
	{
		for (DWORD CurrentProcess = 0; CurrentProcess < ProcessCount; CurrentProcess++)
		{


			//Application must not attack process, PID=0, "System", and application its self
			if (pProcessInfo[CurrentProcess].ProcessId == 0 || strcmp(pProcessInfo[CurrentProcess].pProcessName, "System") == 0
				|| strcmp(pProcessInfo[CurrentProcess].pProcessName, strApplicationName.c_str()) == 0)
				continue;

			DWORD uilProcessID = pProcessInfo[CurrentProcess].ProcessId;
			string strProcessName = pProcessInfo[CurrentProcess].pProcessName;
			BOOL bIs64bitProcess = isProcess64bit(uilProcessID);

			p_refvtProcess.push_back(cProcess(uilProcessID, strProcessName, bIs64bitProcess));
		}
	}
}

