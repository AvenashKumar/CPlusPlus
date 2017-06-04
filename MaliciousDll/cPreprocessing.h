#pragma once
class cPreprocessing
{
public:
	cPreprocessing();
	~cPreprocessing();
	void process();

private:
	void createDestZipFolderAndDeletePreviousFile();
	void deleteDestinationFolder();
};

