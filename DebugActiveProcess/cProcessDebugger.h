#pragma once
#include <Windows.h>
#include <string>
#include "cProcess.h"
#include <map>
#include "cMessages.h"
using namespace std;
//class cDebuggerEvents;
class cProcessDebugger : public cMessages
{
private:
	cProcess m_refProcessInfo;
	const bool m_cbIsKillProcessOnDebugStop;
	const bool m_cbIsDefendProcess;
public:
	cProcessDebugger(const cProcess &p_crefProcessInfo,
		const bool p_cbIsDefendProcess,
		const bool p_cbIsKillProcessOnDebugStop = false);
	~cProcessDebugger();

public:
	bool startProcess();
	bool attachToProcess();
	void debugLoop();

private:
	string getProcessPath();

public:
	class cDebuggerEvents
	{
	private :
		const bool m_cbIsDefendProcess;
	public:
		cDebuggerEvents(const bool p_cbIsDefendProcess);
		~cDebuggerEvents();
		int m_iTotalEvents;

	private:
		map<int, int> m_iiDebugEventCount;
		map<LPVOID, string> m_mapDlls;

	private:
		
		void print(const string &p_crefEventName, const string &p_crefstrMessage);
		string GetFileNameFromHandle(HANDLE hFile);
		bool isMaliciousDllFound(const string &p_crefstrDll);

	public:
		void OnCreateProcessEvent(HANDLE p_hFileHandle);
		void OnCreateThreadDebugEvent(const DEBUG_EVENT &p_crefdbgEvent);
		void OnExitThreadDebugEvent(const DEBUG_EVENT &p_crefdbgEvent);
		void OnExitProcessDebugEvent(const DEBUG_EVENT &p_crefdbgEvent);
		void OnLoadDllDebugEvent(const DEBUG_EVENT &p_crefdbgEvent);
		void OnUnloadDllDebugEvent(const DEBUG_EVENT &p_crefdbgEvent);
		void OnOutputDebugStringEvent(const DEBUG_EVENT &p_crefdbgEvent, const HANDLE &p_processHandle);
		void OnExceptionDebugEvent(const DEBUG_EVENT &p_crefdbgEvent, DWORD &p_refuiContinueStatus);
	private:
		void CountCreateProcessEvent(const bool p_cbIsInc = true);
		void CountCreateThreadDebugEvent(const bool p_cbIsInc = true);
		void CountExitThreadDebugEvent(const bool p_cbIsInc = true);
		void CountLoadDllDebugEvent(const bool p_cbIsInc = true);
		void CountUnloadDllDebugEvent(const bool p_cbIsInc = true);
		void CountOutputDebugStringEvent(const bool p_cbIsInc = true);
		void CountExceptionDebugEvent(const bool p_cbIsInc = true);
	};

private:
	cProcessDebugger::cDebuggerEvents *m_ptrDebugEventsHandler;
};

