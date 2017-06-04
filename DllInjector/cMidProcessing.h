#pragma once
class cUserInputs;
class cMidProcessing
{
private:
	const cUserInputs &m_crefUserInputs;
public:
	cMidProcessing(const cUserInputs &p_refUserInputs);
	~cMidProcessing();

public:
	void process();

private:
};

