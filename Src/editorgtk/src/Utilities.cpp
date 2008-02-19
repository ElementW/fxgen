//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorWidget.cpp
//! \brief	FxGen GTK editor - miscellaneous utility functions
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		07-02-2008
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#include "pch.h"
#include "fxgen_pch.h"

/// Split an FxGen ubyte combo definition into a set of strings
vector<string> parse_fxgen_combo_string(string csv)
{
	vector<string> retval;
	string::size_type first_comma = csv.find(',');
	csv.erase(0, first_comma + 2); // get rid of the default value

	while(csv.size() > 1)
	{
		first_comma = csv.find(',');
		if(first_comma == string::npos)
			first_comma = csv.find(']');
		retval.push_back(csv.substr(0, first_comma));
		csv.erase(0, first_comma + 1);
	}

	return retval;
}

/** Create a web color string from a windows dword color value
\param number An ARGB color value
\return A string of the form #rrggbb
*/
Glib::ustring hex_color(int number)
{
    const char digits[] = "0123456789abcdef";
    char retval[8];
    retval[0]='#';
    retval[7]='\0';

    for (int i = 1; i<7; i+=2)
    {
        int msd = number & 0xf;
        number >>= 4;
        int lsd = number & 0xf;
        retval[i] = digits[lsd];
        retval[i+1] = digits[msd];
        number >>= 4;
    }

    return Glib::ustring(retval);
}


//-----------------------------------------------------------------
//!	\brief	Return First RTClass from a super class name
//!	\author	Johann Nadalutti (fxgen@free.fr)
//-----------------------------------------------------------------
NRTClass* GetFirstClassBySuperClass(char* _pszSuperClassName)
{
	NRTClass* pcurRTC = NRTClass::m_pFirstRTClass;
	while (pcurRTC!=null)
	{
		if (strcmp(pcurRTC->m_pszSuperClassName, _pszSuperClassName) == 0)
			return pcurRTC;
		pcurRTC = pcurRTC->m_pNextRTC;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return Next RTClass from a super class name
//!	\author	Johann Nadalutti (fxgen@free.fr)
//-----------------------------------------------------------------
NRTClass* GetNextClassBySuperClass(char* _pszSuperClassName, NRTClass* _prtclass)
{
	_prtclass = _prtclass->m_pNextRTC;

	while (_prtclass!=null)
	{
		if (strcmp(_prtclass->m_pszSuperClassName, _pszSuperClassName) == 0)
			return _prtclass;

		_prtclass = _prtclass->m_pNextRTC;
	}

	return null;
}

