#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2008";
	static const double UBUNTU_VERSION_STYLE = 8.02;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 5;
	static const long BUILD = 497;
	static const long REVISION = 2745;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1043;
	#define RC_FILEVERSION 0,5,497,2745
	#define RC_FILEVERSION_STRING "0, 5, 497, 2745\0"
	static const char FULLVERSION_STRING[] = "0.5.497.2745";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 97;
	

}
#endif //VERSION_h