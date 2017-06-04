#pragma once
#include <iostream>
#include <vector>
using namespace std;
class cDataCompressor
{
private:
	string m_strDestZipPath;
	vector<string> m_vtstrFilesToBeCompressed;

private:
	cDataCompressor();
public:
	cDataCompressor(const string &p_crefstrCompressDataPath,
		const vector<string>&p_crefvtstrFilesToBeCompressed);

	void compress(const bool p_cbIsHide = false);


private:
	void Zip(const string &p_crefstrDestZipPath, const vector<string> &p_crefvtstrSrcFile);
};