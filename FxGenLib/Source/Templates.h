//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Templates.h
//! \brief	Templates
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
//	Type def
//-----------------------------------------------------------------
typedef int( __cdecl *CompareFnc) (const void *elem1, const void *elem2);

//-----------------------------------------------------------------
//!	\class	NArray
//!	\brief	Template Array Class Definition
//! \note
//!		The return value of CompareFnc is show below in the
//!		relationship of elem1 to elem2:
//!
//!			< 0		if elem1 less than elem2
//!			  0		if elem1 identical to elem2
//!			> 0		if elem1 greater than elem2
//-----------------------------------------------------------------
template < class T >  class __declspec(dllexport) NArray
{
public:

	NArray()
	{
		m_pBuffer = (T*)malloc(sizeof(T)*16); m_dwSize=16; m_dwCount=0; memset(m_pBuffer, 0, 16 * sizeof(T)); m_dwGrow = 16;
	}

	NArray(udword size)
	{
		m_pBuffer = (T*)malloc(sizeof(T)*size); m_dwSize=size; m_dwCount=0; memset(m_pBuffer, 0, size * sizeof(T)); m_dwGrow = 16;
	}

	virtual	~NArray()				{ if (m_pBuffer) free(m_pBuffer);	m_pBuffer=null; }

	//Methods
			udword	Count()			{ return m_dwCount;}	//Return used entries
			udword	Size()			{ return m_dwSize;	}	//Return array's buffer size
			void		Clear()			{ memset(m_pBuffer, 0, m_dwSize * sizeof(T)); m_dwCount=0; }

	//Operators
			T&		operator[]	(udword index)			{ return m_pBuffer[index];		 }

/*
			udword	AddItem(T& item);							//Add an item at the end of the array (Returns the number of items in use)
			void		RemoveItem(udword idx);				//Remove an item
			udword	SetCount(udword c);						//Return count prior to SetCount
			void		SetSize(udword s);
			void		Sort(CompareFnc cmp);					//Sorts the array using the compare function
			void		AddArray(NArray& array);
			udword	Find(T& item);								//Return index if found else -1
			udword	InsertItem(udword idx, T& item);	//Add an item at index into the array (Returns the number of items in use)
			void		SetAtGrow(udword idx, T& item);
*/

			//-----------------------------------------------------------------
			//!	\brief	Add an item at the end of the array
			//!	\return	Returns the number of items in use (prior to adding)
			//-----------------------------------------------------------------
			udword AddItem(const T& item)
			{
				if (m_dwCount>=m_dwSize)
				{
					m_pBuffer = (T*)realloc(m_pBuffer, sizeof(T)*(m_dwSize+m_dwGrow));
					memset(m_pBuffer+m_dwCount, 0, m_dwGrow * sizeof(T));
					m_dwSize+= m_dwGrow;
				}
				m_pBuffer[m_dwCount] = item;
				return m_dwCount++;
			}

			//-----------------------------------------------------------------
			//!	\brief	Set an item at a position and grow array if necessary
			//-----------------------------------------------------------------
			void SetAtGrow(udword idx, T& item)
			{
				if (idx>=m_dwSize)
				{
					m_pBuffer = (T*)realloc(m_pBuffer, sizeof(T)*(idx+m_dwGrow));
					memset(m_pBuffer+idx, 0, m_dwGrow * sizeof(T));
					m_dwSize = idx+m_dwGrow;
				}
				m_pBuffer[idx] = item;
				if (idx>=m_dwCount)	m_dwCount = idx+1;
			}

			//-----------------------------------------------------------------
			//!	\brief	Add an array at the end of the array
			//-----------------------------------------------------------------
			void AddArray(NArray& array)
			{
				udword dwOldCount = m_dwCount;
				SetSize(m_dwCount+array.Count()+m_dwGrow);
				memcpy(m_pBuffer + dwOldCount, array.m_pBuffer, array.m_dwCount * sizeof(T));
				m_dwCount+=array.m_dwCount;
			}


			//-----------------------------------------------------------------
			//!	\brief	Remove an item
			//-----------------------------------------------------------------
			void RemoveItem(udword idx)
			{
				m_dwCount--;
				T* pBuffer = m_pBuffer+idx;

				memcpy(pBuffer, pBuffer+1, (m_dwSize-idx-1) * sizeof(T));
			}

			//-----------------------------------------------------------------
			//!	\brief	Set count
			//-----------------------------------------------------------------
			udword SetCount(udword c)
			{
				if (c>=m_dwSize) {
					m_pBuffer = (T*)realloc(m_pBuffer, sizeof(T)* (c + m_dwGrow));
					m_dwSize = c + m_dwGrow;
				}
				return m_dwCount = c;
			}


			//-----------------------------------------------------------------
			//!	\brief	Set size
			//-----------------------------------------------------------------
			void SetSize(udword s)
			{
				m_pBuffer = (T*)realloc(m_pBuffer, sizeof(T)* s);
				m_dwSize = s;
			}


			//-----------------------------------------------------------------
			//!	\brief	Sorts the array using the compare function
			//-----------------------------------------------------------------
			void Sort(CompareFnc cmp)
			{
				//Quicksort algorithm
				qsort(m_pBuffer, m_dwCount, sizeof(T), cmp);
			}

			//-----------------------------------------------------------------
			//!	\brief	Search an item in array
			//-----------------------------------------------------------------
			udword Find(T& item)
			{
				for (udword i=0; i<m_dwCount; i++)
				{
					if (m_pBuffer[i] == item)
						return i;	//Found
				}

				return -1;	//Not found
			}


			//-----------------------------------------------------------------
			//!	\brief	Add an item at index into the array
			//-----------------------------------------------------------------
			udword InsertItem(udword idx, T& item)
			{
				if (idx>=m_dwSize)
				{
					m_dwSize=idx+m_dwGrow;
					m_pBuffer = (T*)realloc(m_pBuffer, sizeof(T)*m_dwSize);
				}

				T* pBuffer = m_pBuffer+idx;

				memcpy(pBuffer+1, pBuffer, (m_dwSize-idx) * sizeof(T));

				m_pBuffer[idx] = item;
				return m_dwCount++;
			}

			protected:
			//Datas
			T*			m_pBuffer;
			udword	m_dwCount;
			udword	m_dwSize;		//Taille en nombre d'elements
			udword	m_dwGrow;

};
