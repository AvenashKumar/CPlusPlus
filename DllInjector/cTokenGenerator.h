#pragma once
#include <iostream>
#include <Windows.h>
#include "cHandle.h"
using namespace std;

class cTokenGenerator
{
private:
	//Responsible for holding Token handle.
	cHandle m_hToken;

	//Responsible for holding Token Previous Privileges
	TOKEN_PRIVILEGES m_tpTokenPrevPrivileges;

	//Responsible for holding Token Privileges.
	TOKEN_PRIVILEGES m_tpTokenPrivileges;

	//Responsible holding privilege name.
	const string m_cstrPrivilege;
public:

	/*Defaul Constructor.
	*/
	cTokenGenerator(const string &p_crefstrPrivilegeName);

	/*Destructor
	*/
	~cTokenGenerator();

	/*Responsible generating Token
	*/
	void generate();

	/*Responsible closing Access Token
	*/
	//void closeAccessToken();

	/*Responsible for set/unset provided privilege.
	* param p_crefstrPrivilege: Privilege name.
	* param p_cbEnablePrivilege: Enable/Disable.
	*/
	void setPrivilege(const BOOL p_cbEnablePrivilege);


	/** Responsible for returning Locally Unique Identifier(LUID) used on a specified
	system to locally represent the specified privilege name.
	* param p_refLUID: Reference of LUID
	* param p_crefstrPrivilege: A pointer to a null-terminated string that specifies the name of the privilege
	*/
	void getPrivilegeLUID(LUID &p_refLUID, const string &p_crefstrPrivilege);

};