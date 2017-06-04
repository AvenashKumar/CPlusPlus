#include "cMidProcessing.h"
#include "cUserInputs.h"
#include "cDllInjector.h"
#include <vector>
#include "cProcess.h"
#include "sProcessUtils.h"
cMidProcessing::cMidProcessing(const cUserInputs &p_refUserInputs):
m_crefUserInputs(p_refUserInputs)
{
}


cMidProcessing::~cMidProcessing()
{
}

void cMidProcessing::process()
{
	string strMaliciousFunction = m_crefUserInputs.getFuncName();

	cProcess trgProcess = sProcessUtils::getProcess(m_crefUserInputs.getProcessName());

	//code injector
	cDllInjector dllInjector(trgProcess, m_crefUserInputs.getDllPath(), NULL);
	dllInjector.injectDLL();

	//Execute function
	dllInjector.executeFunction(strMaliciousFunction, NULL, 0);

	//while code is not injected
	/*while (!isCodeInjected)
	{


	ptrCodeInjector.reset(new cCodeInjector(trgProcess, strMaliciousDLLPath, NULL));

	isCodeInjected = injectDLL(*ptrCodeInjector, trgProcess, strMaliciousDLLPath);
	}
	ptrCodeInjector->executeFunction(strMaliciousFunction, NULL, 0);*/
}