#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2008";
	static const double UBUNTU_VERSION_STYLE = 8.02;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 5;
	static const long BUILD = 426;
	static const long REVISION = 2361;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 847;
	#define RC_FILEVERSION 0,5,426,2361
	#define RC_FILEVERSION_STRING "0, 5, 426, 2361\0"
	static const char FULLVERSION_STRING[] = "0.5.426.2361";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 26;
	

}
#endif //VERSION_h
