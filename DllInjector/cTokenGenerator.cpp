#pragma once

#include <string>
#include "cTokenGenerator.h"
#include "cHandle.h"
cTokenGenerator::cTokenGenerator(const string &p_crefstrPrivilegeName) :
m_cstrPrivilege(p_crefstrPrivilegeName)
{
}
cTokenGenerator::~cTokenGenerator()
{
	if (m_hToken.getHandle() != NULL)
		setPrivilege(false);
}
void cTokenGenerator::generate()
{
	string strError = "File to generate access token, error code: ";

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &m_hToken.getHandle()))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			if (!ImpersonateSelf(SecurityImpersonation))
			{
				string strErrorWithCode = strError + to_string(GetLastError());
				throw exception(strErrorWithCode.c_str());
			}

			if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &m_hToken.getHandle()))
			{
				string strErrorWithCode = strError + to_string(GetLastError());
				throw exception(strErrorWithCode.c_str());
			}
		}
		else
		{
			string strErrorWithCode = strError + to_string(GetLastError());
			throw exception(strErrorWithCode.c_str());
		}
	}
}


void cTokenGenerator::getPrivilegeLUID(LUID &p_refLUID, const string &p_crefstrPrivilege)
{
	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		p_crefstrPrivilege.c_str(),   // privilege to lookup 
		&p_refLUID))        // receives LUID of privilege
	{
		string strError = "LookupPrivilegeValue error: " + to_string(GetLastError());
		throw exception(strError.c_str());
	}
}

void cTokenGenerator::setPrivilege(const BOOL p_cbEnablePrivilege)
{
	if (m_hToken.getHandle() == NULL)
		throw exception("Token is not generated");

	//Set privilege count
	m_tpTokenPrivileges.PrivilegeCount = 1;

	//Get privilege LUID
	getPrivilegeLUID(m_tpTokenPrivileges.Privileges[0].Luid, m_cstrPrivilege);

	//Enable/Disable privilege.
	if (p_cbEnablePrivilege)
		m_tpTokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		m_tpTokenPrivileges.Privileges[0].Attributes = 0;

	// Enable/Disable all Privileges from generated/provided token.
	if (!AdjustTokenPrivileges(
		m_hToken.getHandle(),
		FALSE,
		&m_tpTokenPrivileges,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		string strError = "AdjustTokenPrivileges error: " + to_string(GetLastError());
		throw exception(strError.c_str());
	}

	/*if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
	throw exception("The token does not have the specified privilege");
	}*/
}