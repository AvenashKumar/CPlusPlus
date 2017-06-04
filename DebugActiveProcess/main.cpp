#include <iostream>
#include <vector>
#include "cCommandReader.h"
#include "cProcessDebugger.h"
#include "sProcessUtils.h"
using namespace std;
int main(const int p_ciTotalArgs, char** p_ptrptrArgs)
{
	try
	{
		vector<string> vtstrArgs;
		for (int i = 1; i < p_ciTotalArgs; i++)
			vtstrArgs.push_back(p_ptrptrArgs[i]);

		if (vtstrArgs.size() < 1)
			throw exception("Invalid number of arguments");

		//Command Reader
		cCommandReader cmdReader(vtstrArgs);
		cmdReader.read();

		//Check errors
		if (cmdReader.isErrorFound())
			return -1;

		//User inputs
		cUserInputs userInputs = cmdReader.getUserInputs();

		//Process Finder
		cProcess trgProcess = sProcessUtils::getProcess(userInputs.getProcessName());

		//Process defender
		cProcessDebugger debugger(trgProcess, userInputs.isDefendProcess());
		if (userInputs.isCreateNewProcess())
		{
			if (!debugger.startProcess())
			{
				debugger.printMessages();
				return -1;
			}
		}
		else
		{
			if (!debugger.attachToProcess())
			{
				debugger.printMessages();
				return -1;
			}
		}

		debugger.debugLoop();

		return 0;
	}
	catch (exception e)
	{
		MessageBox(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	catch (...)
	{
		MessageBox(nullptr, "Unknown", "Error", MB_ICONERROR);
		return -1;
	}
}