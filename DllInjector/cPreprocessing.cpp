#include "cPreprocessing.h"
#include <iostream>
#include "cCommandReader.h"
#include "cUserInputs.h"
cPreprocessing::cPreprocessing(const vector<string> &p_crefvtstrArgs, cUserInputs &p_refUserInputs) :
m_crefvtstrArgs(p_crefvtstrArgs),
m_refUserInputs(p_refUserInputs)
{
}


cPreprocessing::~cPreprocessing()
{
}

void cPreprocessing::process()
{
	readCommands();
}


void cPreprocessing::printErrorMessages(const vector<string> &p_crefvtstrMessages)
{
	for (string strMessage : p_crefvtstrMessages)
	{
		cout << strMessage << endl;
	}
}

void cPreprocessing::readCommands()
{
	//Read cmd arguments
	cCommandReader cmdReader(m_crefvtstrArgs);
	cmdReader.read();

	//Get Error/warning Messages and Print
	vector<string> vtstrMessages = cmdReader.getMessages();

	//Print Messages
	printErrorMessages(vtstrMessages);

	if (cmdReader.isErrorFound())
		throw exception("One or more error(s) found in input arguments");

	//Get User Inputs
	m_refUserInputs = cmdReader.getUserInputs();
}


