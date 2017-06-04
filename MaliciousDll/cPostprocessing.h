#pragma once
class cPostprocessing
{
public:
	cPostprocessing();
	~cPostprocessing();
	void process();

private:
	void deleteDestinationFolder();
};

