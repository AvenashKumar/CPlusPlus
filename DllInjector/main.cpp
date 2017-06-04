#include <iostream>
#include "cPreprocessing.h"
#include "cMidProcessing.h"
#include "sConstants.h"
#include "cUserInputs.h"
using namespace std;


int main(int argc, char *argv[])
{
	try
	{
		//Get cmd arguments
		vector<string> vtstrArgs;
		for (int i = 1; i < argc; ++i)
		{
			vtstrArgs.push_back(argv[i]);
		}

		//Precessing
		cUserInputs userInputs;
		cPreprocessing preProcessing(vtstrArgs, userInputs);
		preProcessing.process();

		//Mid Processing
		cMidProcessing midProcessing(userInputs);
		midProcessing.process();
	}
	catch (exception e)
	{
		cout << sConstants::STR_ERROR + e.what() << endl;
		return -1;
	}
	catch (...)
	{
		cout << sConstants::STR_ERROR+"Unknown error" << endl;
		return -1;
	}
}