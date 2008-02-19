//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Stream.cpp
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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Stream.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NMemoryStream class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NMemoryStream::NMemoryStream()
{
	m_pbyBuffer			= null;
	m_dwBufSize     = 0;
	m_dwBufPos      = 0;
	m_bManagedBuffer= true;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NMemoryStream::~NMemoryStream()
{
  Close();
}

//-----------------------------------------------------------------
//!	\brief	Open a memory stream for reading and writing
//!	\param	buffer	    Memory buffer to write to, or null to make NMemoryStream manage the buffer
//!	\param	bufferSize  Size of user buffer, if supplied
//!	\return	true on success
//-----------------------------------------------------------------
bool NMemoryStream::Open(ubyte *_buffer, udword _bufferSize)
{
  Close();

  if (_buffer != null)
  {
    m_bManagedBuffer = false;
    m_dwBufSize = _bufferSize;
    m_pbyBuffer = _buffer;
  } else {
    m_bManagedBuffer = true;
    m_dwBufSize	= NSF_BUFSTARTSIZE;
    m_pbyBuffer	= (ubyte*)NMemAlloc(m_dwBufSize);
  }
  
  m_dwBufPos = 0;
  return true;
}


//-----------------------------------------------------------------
//!	\brief	Close the memory stream
//-----------------------------------------------------------------
void NMemoryStream::Close()
{
  if (m_bManagedBuffer && m_pbyBuffer != null)
  {
    NMemFree(m_pbyBuffer);
  }

  m_pbyBuffer	      = null;
  m_dwBufSize         = 0;
  m_dwBufPos          = 0;
  m_bManagedBuffer    = true;
}



//-----------------------------------------------------------------
//!	\brief	Write data into the stream
//!	\param	_buf		Buffer to write from
//!	\param	_length		Number of bytes to write
//-----------------------------------------------------------------

bool NMemoryStream::PutData(const void* _buf, udword _length)
{
	//Grow buffer size if necessary, and we're managing it
	if ((m_dwBufPos+_length)>=m_dwBufSize)
	{
		if (m_bManagedBuffer)
		{
			m_dwBufSize+=NSF_BUFGROWSIZE+_length;
			m_pbyBuffer=(ubyte*)NMemRealloc(m_pbyBuffer,m_dwBufSize);
		} else {
			return false;
		}
	}

	//Save datas
	memcpy(m_pbyBuffer+m_dwBufPos, _buf, _length);
	m_dwBufPos+=_length;

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Read data from the stream
//!	\param	_buf		Buffer to read into
//!	\param	_length		Number of bytes to read
//-----------------------------------------------------------------
bool NMemoryStream::GetData(void* _buf, udword _length)
{
  if ((m_dwBufPos+_length)>m_dwBufSize)
    return false;

  memcpy(_buf, m_pbyBuffer+m_dwBufPos, _length);
  m_dwBufPos += _length;
  return true;
}


//-----------------------------------------------------------------
//!	\brief	Seek to a specific position in the stream
//!	\param	_position	The position to seek to
//-----------------------------------------------------------------
bool NMemoryStream::Seek(udword _position)
{
  if (_position <= m_dwBufSize)
  {
    m_dwBufPos = _position;
    return true;
  }

  return false;
}


//-----------------------------------------------------------------
//!	\brief	Returns the current position in the stream
//!     \return Current position in the stream
//-----------------------------------------------------------------
udword NMemoryStream::Tell()
{
  return m_dwBufPos;
}






//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NFileStream class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NFileStream::NFileStream()
{
  m_pFile = null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NFileStream::~NFileStream()
{
  Close();
}

//-----------------------------------------------------------------
//!	\brief	Open a file stream for reading or writing
//!	\param	_filename	Full file name
//!	\param	_writing	true to open for writing
//!	\return	true on success
//-----------------------------------------------------------------
bool NFileStream::Open(const char* _filename, bool _writing)
{
  m_pFile = fopen(_filename, _writing ? "wb" : "rb");
  
  return m_pFile != null;
}

//-----------------------------------------------------------------
//!	\brief	Close the file stream
//-----------------------------------------------------------------
void NFileStream::Close()
{
  if (m_pFile != null)
  {
    fclose(m_pFile);
    m_pFile = null;
  }
}

//-----------------------------------------------------------------
//!	\brief	Write data into the stream
//!	\param	_buf		Buffer to write from
//!	\param	_length		Number of bytes to write
//-----------------------------------------------------------------

bool NFileStream::PutData(const void* _buf, udword _length)
{
  return fwrite(_buf, _length, 1, m_pFile) == 1;
}


//-----------------------------------------------------------------
//!	\brief	Read data from the stream
//!	\param	_buf		Buffer to read into
//!	\param	_length		Number of bytes to read
//-----------------------------------------------------------------
bool NFileStream::GetData(void* _buf, udword _length)
{
  return fread(_buf, _length, 1, m_pFile) == 1;
}


//-----------------------------------------------------------------
//!	\brief	Seek to a specific position in the stream
//!	\param	_position	The position to seek to
//-----------------------------------------------------------------
bool NFileStream::Seek(udword _position)
{
  return fseek(m_pFile, _position, SEEK_SET) == 0;
}


//-----------------------------------------------------------------
//!	\brief	Returns the current position in the stream
//!     \return Current position in the stream
//-----------------------------------------------------------------
udword NFileStream::Tell()
{
  return ftell(m_pFile);
}
