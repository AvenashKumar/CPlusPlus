#pragma once
#include <vector>
using namespace std;
class cPackageCreator
{
public:
	cPackageCreator();
	~cPackageCreator();

public:
	void packageCookies(const vector<string> &p_crefvtstrPaths);

private:
	void createPackage(const string &p_crefstrFilePath, const vector<string> &p_crefvtstrPaths);
};

