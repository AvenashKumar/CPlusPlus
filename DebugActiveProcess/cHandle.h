#pragma once
#include <Windows.h>
class cHandle
{
private:
	HANDLE m_hHandle;
public:
	cHandle();
	cHandle(const HANDLE& p_hHandle);
	HANDLE& getHandle();
	void setHandle(const HANDLE& p_refhHandle);
	~cHandle();
};