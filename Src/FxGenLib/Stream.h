//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Stream.h
//! \brief	Serializer for datas loading and saving
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!             Anders Stenberg (anders.stenberg@gmail.com)
//!
//!	\date		27-05-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"

//-----------------------------------------------------------------
//	Prototypes
//-----------------------------------------------------------------
class NObject;

//-----------------------------------------------------------------
//!	\class	NStream
//!	\brief	Base class for loading and saving datas
//-----------------------------------------------------------------
class NStream
{
public: 
	virtual bool	PutData(const void* _buf, udword _length) = 0;
	virtual bool	GetData(void* _buf, udword _length) = 0;

	virtual bool Seek(udword _position) = 0;
	virtual udword Tell() = 0;

	NStream &operator<<( const uword &_val	)	{ PutData(&_val, sizeof(uword)); return *this;}
	NStream &operator<<( const sword &_val	)	{ PutData(&_val, sizeof(sword)); return *this;}
	NStream &operator<<( const udword &_val )	{ PutData(&_val, sizeof(udword)); return *this;}
	NStream &operator<<( const sdword &_val )	{ PutData(&_val, sizeof(sdword)); return *this;}
	NStream &operator<<( const ubyte &_val	)	{ PutData(&_val, sizeof(ubyte)); return *this;}
	NStream &operator<<( const sbyte &_val	)	{ PutData(&_val, sizeof(sbyte)); return *this;}
	NStream &operator<<( const float &_val	)	{ PutData(&_val, sizeof(float)); return *this;}
	NStream &operator<<( const bool &_val	)	{ PutData(&_val, sizeof(bool)); return *this;}
	NStream &operator<<( const char* &_val	)	{ uword l=(uword)strlen(_val); PutData(&l, sizeof(uword)); PutData(_val, l); return *this;}

	NStream &operator>>( uword &_val )	{ GetData(&_val, sizeof(uword)); return *this;}
	NStream &operator>>( sword &_val )	{ GetData(&_val, sizeof(sword)); return *this;}
	NStream &operator>>( udword &_val)	{ GetData(&_val, sizeof(udword)); return *this;}
	NStream &operator>>( sdword &_val)	{ GetData(&_val, sizeof(sdword)); return *this;}
	NStream &operator>>( ubyte &_val )	{ GetData(&_val, sizeof(ubyte)); return *this;}
	NStream &operator>>( sbyte &_val )	{ GetData(&_val, sizeof(sbyte)); return *this;}
	NStream &operator>>( float &_val )	{ GetData(&_val, sizeof(float)); return *this;}
	NStream &operator>>( bool &_val	 )	{ GetData(&_val, sizeof(bool)); return *this;}
	NStream &operator>>( char* _val	 )	{ uword l; GetData(&l, sizeof(uword)); GetData(_val, l); _val[l]=0; return *this;}
};

//-----------------------------------------------------------------
//!	\class	NFileStream
//!	\brief	Serializer for loading and saving to file
//-----------------------------------------------------------------
class NFileStream : public NStream
{
public:
  NFileStream();
  virtual ~NFileStream();

	//File Methods
	bool		Open(const char* _filename, bool _writing=false);
	void		Close();

	virtual bool	PutData(const void* _buf, udword _length);
	virtual bool	GetData(void* _buf, udword _length);

  virtual bool		Seek(udword _position);
  virtual udword	Tell();

protected:
 	//Data
	FILE*			m_pFile;
};

//-----------------------------------------------------------------
//!	\class	NMemoryStream
//!	\brief	Serializer for loading and saving to a memory buffer
//-----------------------------------------------------------------
class NMemoryStream : public NStream
{
public:
	NMemoryStream();
	virtual ~NMemoryStream();

	bool    Open(ubyte *_buffer = null, udword _bufferSize = 0);
	void    Close();
        
	bool	PutData(const void* _buf, udword _length);
	bool	GetData(void* _buf, udword _length);

	virtual bool Seek(udword position);
	virtual udword Tell();

	ubyte* GetBuffer() { return m_pbyBuffer; }
	udword GetBufferSize() { return m_dwBufSize; }

protected:
	ubyte*		m_pbyBuffer;
	udword		m_dwBufSize;
	udword		m_dwBufPos;
	bool      m_bManagedBuffer;
};

