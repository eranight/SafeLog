#include "SafeLog.h"
#include <iostream>
#include <string>

int main()
{
	using namespace safelog;
	SafeLog log("C:/log.log");
	log(SL_ERROR) << "some error!";
	log(SL_WARNING) << "some warrning!";
	log(SL_DEBUG) << "some debug!";

	return 0;
}