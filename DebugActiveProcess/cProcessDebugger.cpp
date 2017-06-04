
#include "cProcessDebugger.h"
#include <string>
#include <Windows.h>
#include <map>
#include<Dbghelp.h>
#include <psapi.h>
#include "shlwapi.h"
#include <iostream>
#include <WinBase.h>
#include "sPathUtils.h"
using namespace std;

cProcessDebugger::cProcessDebugger(const cProcess &p_crefProcessInfo,
	const bool p_cbIsDefendProcess,
	const bool p_cbIsKillProcessOnDebugStop /* = false */) :
m_refProcessInfo(p_crefProcessInfo),
m_cbIsDefendProcess(p_cbIsDefendProcess),
m_cbIsKillProcessOnDebugStop(p_cbIsKillProcessOnDebugStop)
{
	m_ptrDebugEventsHandler = new cProcessDebugger::cDebuggerEvents(m_cbIsDefendProcess);
}

cProcessDebugger::~cProcessDebugger()
{
	if (m_ptrDebugEventsHandler != NULL)
	{
		delete m_ptrDebugEventsHandler;
		m_ptrDebugEventsHandler = NULL;
	}
}

string cProcessDebugger::getProcessPath()
{
	HANDLE processHandle = NULL;
	TCHAR filename[MAX_PATH];

	processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_refProcessInfo.getProcessID());
	if (processHandle != NULL) 
	{
		if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0) 
		{
			string strError = "Fail to get process: " + m_refProcessInfo.getProcessName() + " path";
			CloseHandle(processHandle);
			throw exception(strError.c_str());
		}
		CloseHandle(processHandle);
	}
	else 
	{
		throw exception("Failed to open process.");
	}
	return filename;
}

bool cProcessDebugger::startProcess()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	string strProcessPath = getProcessPath();

	if (!CreateProcess(strProcessPath.c_str(), NULL, NULL, NULL, false, DEBUG_ONLY_THIS_PROCESS,
		NULL, NULL, &si, &pi))
	{
		DWORD uiError = GetLastError();
		if (uiError == ERROR_NOT_SUPPORTED)
			addErrorMessage("Unable to debug 64-bit process from 32-bit exe");
		else
			addErrorMessage("CreateProcess() failed. Error: " + GetLastError());

		return false;
	}

	m_refProcessInfo.getProcessHandle().setHandle(pi.hProcess);

	return true;
}
bool cProcessDebugger::attachToProcess()
{
	if (!DebugActiveProcess(m_refProcessInfo.getProcessID()))
	{
		DWORD uiError = GetLastError();
		if (uiError == ERROR_NOT_SUPPORTED)
			addErrorMessage("Unable to debug 64-bit process from 32-bit exe");
		else
			addErrorMessage("DebugActiveProcess() failed. Error:" + GetLastError());

		return false;
	}
	return true;
}

void cProcessDebugger::debugLoop()
{
	/*BOOL bIsCreated = false;
	if (m_cbIsCreateProcess)
	{
		string strProcessPath = getProcessPath();
		
		bIsCreated=CreateProcess(strProcessPath.c_str(), NULL, NULL, NULL, false, DEBUG_ONLY_THIS_PROCESS,
			NULL, NULL, &si, &pi);
	}
	else
		bIsCreated = DebugActiveProcess(m_crefProcessInfo.getProcessID());
	
	if (!bIsCreated)
	{
		DWORD uiError=GetLastError();
		if (uiError == ERROR_NOT_SUPPORTED)
		{
			throw exception("Unable to debug 64bit process from 32bit exe");
		}
		else
		{
			string strError = "Unable to debug " + m_crefProcessInfo.getProcessName() + " Error Code:=" + to_string(uiError);
			throw exception(strError.c_str());
		}
	}
	else
	{
		
	}*/

	if (!DebugSetProcessKillOnExit(m_cbIsKillProcessOnDebugStop))
	{
		if (!m_cbIsKillProcessOnDebugStop)
			addWarningMessage("Debugger will kill active process, on terminating this process");

		printMessages();
	}

	string strEventMessage;
	std::map<LPVOID, string> DllNameMap;


	DEBUG_EVENT debug_event = { 0 };

	bool bContinueDebugging = true;

	DWORD dwContinueStatus = DBG_CONTINUE;

	while (bContinueDebugging)
	{
		if (!WaitForDebugEvent(&debug_event, INFINITE))
			return;

		switch (debug_event.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnCreateProcessEvent(debug_event.u.CreateProcessInfo.hFile);
			break;

		case CREATE_THREAD_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnCreateThreadDebugEvent(debug_event);
			break;
		case EXIT_THREAD_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnExitThreadDebugEvent(debug_event);
			break;

		case EXIT_PROCESS_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnExitProcessDebugEvent(debug_event);
			bContinueDebugging = false;
			break;

		case LOAD_DLL_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnLoadDllDebugEvent(debug_event);
			break;

		case UNLOAD_DLL_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnUnloadDllDebugEvent(debug_event);
			break;

		case OUTPUT_DEBUG_STRING_EVENT:
			m_ptrDebugEventsHandler->OnOutputDebugStringEvent(debug_event, m_refProcessInfo.getProcessHandle().getHandle());
			break;

		case EXCEPTION_DEBUG_EVENT:
			m_ptrDebugEventsHandler->OnExceptionDebugEvent(debug_event, dwContinueStatus);
			break;
		}

		//OnDebugEventMessage(strEventMessage, debug_event.dwDebugEventCode);
		//SendMessage(DEBUG_EVENT_MESSAGE, (WPARAM)&strEventMessage, debug_event.dwDebugEventCode);

		ContinueDebugEvent(debug_event.dwProcessId,
			debug_event.dwThreadId,
			dwContinueStatus);

		// Reset
		dwContinueStatus = DBG_CONTINUE;
	}
}

cProcessDebugger::cDebuggerEvents::cDebuggerEvents(const bool p_cbIsDefendProcess) :
m_cbIsDefendProcess(p_cbIsDefendProcess)
{

	m_iiDebugEventCount.insert(pair<int, int>(CREATE_PROCESS_DEBUG_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(CREATE_THREAD_DEBUG_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(EXIT_THREAD_DEBUG_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(EXIT_PROCESS_DEBUG_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(LOAD_DLL_DEBUG_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(UNLOAD_DLL_DEBUG_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(OUTPUT_DEBUG_STRING_EVENT, 0));
	m_iiDebugEventCount.insert(pair<int, int>(EXCEPTION_DEBUG_EVENT, 0));
	m_iTotalEvents = 0;
}


cProcessDebugger::cDebuggerEvents::~cDebuggerEvents()
{
}

void cProcessDebugger::cDebuggerEvents::CountCreateProcessEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[CREATE_PROCESS_DEBUG_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[CREATE_PROCESS_DEBUG_EVENT] = iCount;
}
void cProcessDebugger::cDebuggerEvents::CountCreateThreadDebugEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[CREATE_THREAD_DEBUG_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[CREATE_THREAD_DEBUG_EVENT] = iCount;
}

void cProcessDebugger::cDebuggerEvents::CountExitThreadDebugEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[EXIT_THREAD_DEBUG_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[EXIT_THREAD_DEBUG_EVENT] = iCount;
}
void cProcessDebugger::cDebuggerEvents::CountLoadDllDebugEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[LOAD_DLL_DEBUG_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[LOAD_DLL_DEBUG_EVENT] = iCount;
}
void cProcessDebugger::cDebuggerEvents::CountUnloadDllDebugEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[UNLOAD_DLL_DEBUG_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[UNLOAD_DLL_DEBUG_EVENT] = iCount;
}
void cProcessDebugger::cDebuggerEvents::CountOutputDebugStringEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[OUTPUT_DEBUG_STRING_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[OUTPUT_DEBUG_STRING_EVENT] = iCount;
}
void cProcessDebugger::cDebuggerEvents::CountExceptionDebugEvent(const bool p_cbIsInc)
{
	int iCount = m_iiDebugEventCount[EXCEPTION_DEBUG_EVENT];
	if (p_cbIsInc)
		++iCount;
	else
		--iCount;
	m_iiDebugEventCount[EXCEPTION_DEBUG_EVENT] = iCount;
}


string cProcessDebugger::cDebuggerEvents::GetFileNameFromHandle(HANDLE hFile)
{
	BOOL bSuccess = FALSE;
	char pszFilename[MAX_PATH + 1];
	HANDLE hFileMap;

	string strFilename;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
		return FALSE;
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);

	if (hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(),
				pMem,
				pszFilename,
				MAX_PATH))
			{

				// Translate path with device name to drive letters.
				const int BUFSIZE = 512;
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';

				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = strlen(szName);

							if (uNameLen < MAX_PATH)
							{
								string strSubStr = string(pszFilename).substr(0, uNameLen);
								bFound = strcmp(strSubStr.c_str(), szName) == 0;

								if (bFound)
								{
									//strFilename.replace(0, 23, "");
									strFilename = szDrive + string(pszFilename).replace(0, uNameLen, "");

									//strFilename.Format(L"%s%s", szDrive, pszFilename + uNameLen);
								}
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}

	return(strFilename);
}

void cProcessDebugger::cDebuggerEvents::print(const string &p_crefEventName, const string &p_crefstrMessage)
{
	cout << "Total Events: " + to_string(m_iTotalEvents)
		+ " Event: " + p_crefEventName
		+ " Message: " + p_crefstrMessage << endl;
}
void cProcessDebugger::cDebuggerEvents::OnCreateProcessEvent(HANDLE p_hFileHandle)
{
	string strFileName = GetFileNameFromHandle(p_hFileHandle);
	m_iTotalEvents++;
	print("CREATE_PROCESS_DEBUG_EVENT", strFileName);
}

void cProcessDebugger::cDebuggerEvents::OnCreateThreadDebugEvent(const DEBUG_EVENT &p_crefdbgEvent)
{

	char strEventMessage[100];
	sprintf_s(strEventMessage,
		"Thread 0x%x (Id: %d) created at: 0x%x",
		p_crefdbgEvent.u.CreateThread.hThread,
		p_crefdbgEvent.dwThreadId,
		p_crefdbgEvent.u.CreateThread.lpStartAddress);// Thread 0xc (Id: 7920) created at: 0x77b15e58

	CountCreateThreadDebugEvent();
	m_iTotalEvents++;
	print("CREATE_THREAD_DEBUG_EVENT", strEventMessage);
}

void cProcessDebugger::cDebuggerEvents::OnExitThreadDebugEvent(const DEBUG_EVENT &p_crefdbgEvent)
{
	char strEventMessage[100] = "";
	sprintf_s(strEventMessage, "The thread %d exited with code: %d",
		p_crefdbgEvent.dwThreadId,
		p_crefdbgEvent.u.ExitThread.dwExitCode); // The thread 2760 exited with code: 0

	CountCreateThreadDebugEvent(false);

	m_iTotalEvents++;

	print("EXIT_THREAD_DEBUG_EVENT", strEventMessage);
}

void cProcessDebugger::cDebuggerEvents::OnExitProcessDebugEvent(const DEBUG_EVENT &p_crefdbgEvent)
{
	char strEventMessage[60];
	sprintf_s(strEventMessage, "0x%x",
		p_crefdbgEvent.u.ExitProcess.dwExitCode);

	print("EXIT_PROCESS_DEBUG_EVENT", strEventMessage);

}
void cProcessDebugger::cDebuggerEvents::OnLoadDllDebugEvent(const DEBUG_EVENT &p_crefdbgEvent)
{
	string strDllFilePath = GetFileNameFromHandle(p_crefdbgEvent.u.LoadDll.hFile);

	string strEventMessage = strDllFilePath;

	m_mapDlls.insert(
		std::make_pair(p_crefdbgEvent.u.LoadDll.lpBaseOfDll, strEventMessage));

	char strDllBaseAddress[50];
	sprintf_s(strDllBaseAddress,
		"%x",
		p_crefdbgEvent.u.LoadDll.lpBaseOfDll);


	strEventMessage += strDllBaseAddress;

	CountLoadDllDebugEvent();

	m_iTotalEvents++;

	print("LOAD_DLL_DEBUG_EVENT", strEventMessage);

	if (m_cbIsDefendProcess && isMaliciousDllFound(strDllFilePath))
	{
		string strError = "Malicious dll found at path: " + strDllFilePath;
		throw exception(strError.c_str());
	}
}
void cProcessDebugger::cDebuggerEvents::OnUnloadDllDebugEvent(const DEBUG_EVENT &p_crefdbgEvent)
{
	string strEventMessage = m_mapDlls[p_crefdbgEvent.u.UnloadDll.lpBaseOfDll];
	CountLoadDllDebugEvent(false);
	m_iTotalEvents++;
	print("UNLOAD_DLL_DEBUG_EVENT", strEventMessage);
}
void cProcessDebugger::cDebuggerEvents::OnOutputDebugStringEvent(const DEBUG_EVENT &p_crefdbgEvent, const HANDLE &p_processHandle)
{
	const OUTPUT_DEBUG_STRING_INFO & DebugString = p_crefdbgEvent.u.DebugString;

	char *msg = new char[DebugString.nDebugStringLength];
	ZeroMemory(msg, DebugString.nDebugStringLength);

	ReadProcessMemory(p_processHandle, DebugString.lpDebugStringData, msg, DebugString.nDebugStringLength, NULL);

	string strEventMessage = "";
	if (DebugString.fUnicode)
		strEventMessage = msg;
	else
		strEventMessage = msg;

	delete[]msg;
	msg = NULL;

	CountOutputDebugStringEvent();
	m_iTotalEvents++;

	print("OUTPUT_DEBUG_STRING_EVENT", strEventMessage);
}
void cProcessDebugger::cDebuggerEvents::OnExceptionDebugEvent(const DEBUG_EVENT &p_crefdbgEvent, DWORD &p_refuiContinueStatus)
{
	const EXCEPTION_DEBUG_INFO& exception = p_crefdbgEvent.u.Exception;
	string strEventMessage = "";
	switch (exception.ExceptionRecord.ExceptionCode)
	{
	case STATUS_BREAKPOINT:
		strEventMessage = "Break point";
		break;

	default:
		if (exception.dwFirstChance == 1)
		{
			char strEventMessage1[100];
			sprintf_s(strEventMessage1,
				"First chance exception at %x, exception-code: 0x%08x",
				exception.ExceptionRecord.ExceptionAddress,
				exception.ExceptionRecord.ExceptionCode);
			strEventMessage = strEventMessage1;
		}
		// else
		// { Let the OS handle }


		// There are cases where OS ignores the dwContinueStatus, 
		// and executes the process in its own way.
		// For first chance exceptions, this parameter is not-important
		// but still we are saying that we have NOT handled this event.

		// Changing this to DBG_CONTINUE (for 1st chance exception also), 
		// may cause same debugging event to occur continously.
		// In short, this debugger does not handle debug exception events
		// efficiently, and let's keep it simple for a while!
		p_refuiContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
	}

	CountExceptionDebugEvent();
	m_iTotalEvents++;
	print("EXCEPTION_DEBUG_EVENT", strEventMessage);
}

bool cProcessDebugger::cDebuggerEvents::isMaliciousDllFound(const string &p_crefstrDllPath)
{
	string strDLLName = "";
	sPathUtils::getFileName(p_crefstrDllPath, strDLLName);

	if (strDLLName == "MaliciousDll.dll")
		return true;

	return false;
}