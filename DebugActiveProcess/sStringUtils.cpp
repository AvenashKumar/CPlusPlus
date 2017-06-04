#pragma once
#include "sStringUtils.h"
#include <algorithm>


void sStringUtils::tokenize(const string &p_crefstrInputString, vector<string> &p_refvtTokens)
{
	string strTempInputString = trim(p_crefstrInputString);

	if (strTempInputString.empty())
		return;

	if (*strTempInputString.begin() != '"')
	{
		size_t uiSpacePos = strTempInputString.find(" ");
		if (uiSpacePos != string::npos)
		{
			p_refvtTokens.push_back(strTempInputString.substr(0, uiSpacePos));

			//Avoid extra space that's why plus one
			tokenize(strTempInputString.substr(uiSpacePos + 1), p_refvtTokens);
		}
		else
			p_refvtTokens.push_back(strTempInputString);

	}
	else
	{
		//Find double quotes after 1st character
		size_t uiQuotesPos = strTempInputString.find('"', 1);
		if (uiQuotesPos != string::npos)
		{
			//No need to insert quotes
			p_refvtTokens.push_back(strTempInputString.substr(1, uiQuotesPos - 1));
			//Avoid ending double quotes from next token string
			tokenize(strTempInputString.substr(uiQuotesPos + 1), p_refvtTokens);
		}
		else
			p_refvtTokens.push_back(strTempInputString);
	}
}
string sStringUtils::trim(const string& p_crefstrInput)
{
	size_t uiPosSpace = p_crefstrInput.find_first_not_of(' ');
	if (string::npos == uiPosSpace)
	{
		return p_crefstrInput;
	}
	size_t uiEndPosSpace = p_crefstrInput.find_last_not_of(' ');
	return p_crefstrInput.substr(uiPosSpace, (uiEndPosSpace - uiPosSpace + 1));
}
string sStringUtils::toUpper(const string &p_refstrInputString)
{
	string strUpperString = "";
	strUpperString.resize(p_refstrInputString.size());
	std::transform(p_refstrInputString.begin(), p_refstrInputString.end(), strUpperString.begin(), ::towupper);
	return strUpperString;
}