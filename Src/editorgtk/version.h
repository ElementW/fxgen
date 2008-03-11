#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2008";
	static const double UBUNTU_VERSION_STYLE = 8.03;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 6;
	static const long BUILD = 506;
	static const long REVISION = 2782;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1065;
	#define RC_FILEVERSION 0,6,506,2782
	#define RC_FILEVERSION_STRING "0, 6, 506, 2782\0"
	static const char FULLVERSION_STRING[] = "0.6.506.2782";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 6;
	

}
#endif //VERSION_h
