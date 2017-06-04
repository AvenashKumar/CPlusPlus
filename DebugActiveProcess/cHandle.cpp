#pragma once
#include <iostream>
#include <string>
using namespace std;
#include "cHandle.h"
cHandle::cHandle() :
m_hHandle(NULL)
{

}
cHandle::cHandle(const HANDLE& p_hHandle) :
m_hHandle(p_hHandle)
{
}
HANDLE& cHandle::getHandle()
{
	return m_hHandle;
}

void cHandle::setHandle(const HANDLE& p_refhHandle)
{
	m_hHandle = p_refhHandle;
}
cHandle::~cHandle()
{
	if (m_hHandle == NULL)
		return;

	if (!CloseHandle(m_hHandle))
	{
		string strError = "Unable to Close Handle, Error Code -> " + to_string(GetLastError());
		throw exception(strError.c_str());
	}
}