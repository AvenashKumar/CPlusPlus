#pragma once
#include <vector>
#include "cProcess.h"
using namespace std;
class sProcessUtils
{
public:
	static bool isProcessExist(const string &p_crefvtstrProcessName);
	static void getAllProcess(vector<cProcess> & p_refvtProcess);
	static BOOL isProcess64bit(const DWORD p_uilProcessID);
	static cProcess getProcess(const string &p_refstrProcessName);
};

