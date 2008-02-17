#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2008";
	static const double UBUNTU_VERSION_STYLE = 8.02;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 3;
	static const long BUILD = 276;
	static const long REVISION = 1529;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 604;
	#define RC_FILEVERSION 0,3,276,1529
	#define RC_FILEVERSION_STRING "0, 3, 276, 1529\0"
	static const char FULLVERSION_STRING[] = "0.3.276.1529";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 76;
	

}
#endif //VERSION_h
