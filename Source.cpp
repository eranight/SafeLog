#include "SafeLog.h"
#include <iostream>
#include <string>

int main()
{
	SafeLog log("C:/log.log");
	log(SL_ERROR) << "some error!";
	log(SL_WARNING) << "some warrning!";
	log(SL_DEBUG) << "some error!";

	return 0;
}