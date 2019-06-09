/*
* Name: QinKuai
* Status: Finished
*/
#include "log.h"

std::string getTime() {
	time_t time_p;
	struct tm p;
	char date[50];
	memset(date, 0, sizeof(date));

	time(&time_p);
	localtime_s(&p, &time_p);
	sprintf_s(date, "%02d/%02d/%02d %s %02d:%02d:%02d", p.tm_year + 1900, p.tm_mon + 1, p.tm_mday,
		wdays[p.tm_wday].c_str(), p.tm_hour, p.tm_min, p.tm_sec);

	return std::string(date);
}