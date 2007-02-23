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
template < class T > class NArray
{
public:

	NArray()
	{
		mBuffer = (T*)malloc(sizeof(T)*16); mSize=16; mCount=0; memset(mBuffer, 0, 16 * sizeof(T)); mGrow = 16;
	}

	NArray(udword size)
	{
		mBuffer = (T*)malloc(sizeof(T)*size); mSize=size; mCount=0; memset(mBuffer, 0, size * sizeof(T)); mGrow = 16;
	}

	virtual	~NArray()				{ if (mBuffer) free(mBuffer);	mBuffer=null; }

	//Methods
			udword	Count()			{ return mCount;}	//Return used entries
			udword	Size()			{ return mSize;	}	//Return array's buffer size
			udword	AddItem(T& item);							//Add an item at the end of the array (Returns the number of items in use)
			void		RemoveItem(udword idx);				//Remove an item
			udword	SetCount(udword c);						//Return count prior to SetCount
			void		SetSize(udword s);
			void		Sort(CompareFnc cmp);					//Sorts the array using the compare function
			void		Clear()			{ memset(mBuffer, 0, mSize * sizeof(T)); mCount=0; }
			void		AddArray(NArray& array);
			udword	Find(T& item);								//Return index if found else -1
			udword	InsertItem(udword idx, T& item);	//Add an item at index into the array (Returns the number of items in use)
			void		SetAtGrow(udword idx, T& item);

	//Operators
			T&		operator[]	(udword index)			{ return mBuffer[index];		 }

protected:
	//Datas
			T*			mBuffer;
			udword	mCount;
			udword	mSize;		//Taille en nombre d'elements
			udword	mGrow;
};


//-----------------------------------------------------------------
//!	\brief	Add an item at the end of the array
//!	\return	Returns the number of items in use (prior to adding)
//-----------------------------------------------------------------
template <class T>	udword NArray<T>::AddItem(T& item)
{
	if (mCount>=mSize)
	{
		mBuffer = (T*)realloc(mBuffer, sizeof(T)*(mSize+mGrow));
		mSize+= mGrow;
	}
	mBuffer[mCount] = item;
	return mCount++;
}

//-----------------------------------------------------------------
//!	\brief	Set an item at a position and grow array if necessary
//-----------------------------------------------------------------
template <class T>	void NArray<T>::SetAtGrow(udword idx, T& item)
{
	if (idx>=mSize)
	{
		mSize = idx+mGrow;
		mBuffer = (T*)realloc(mBuffer, sizeof(T)*(mSize));
	}
	mBuffer[idx] = item;
	if (idx>=mCount)	mCount = idx+1;
}

//-----------------------------------------------------------------
//!	\brief	Add an array at the end of the array
//-----------------------------------------------------------------
template <class T>	void NArray<T>::AddArray(NArray& array)
{
	udword dwOldCount = mCount;
	SetSize(mCount+array.Count()+mGrow);
	CopyMemory(mBuffer + dwOldCount, array.mBuffer, array.mCount * sizeof(T));
	mCount+=array.mCount;
}


//-----------------------------------------------------------------
//!	\brief	Remove an item
//-----------------------------------------------------------------
template <class T>	void NArray<T>::RemoveItem(udword idx)
{
	mCount--;
	T* pBuffer = mBuffer+idx;

	CopyMemory(pBuffer, pBuffer+1, (mSize-idx-1) * sizeof(T));

}

//-----------------------------------------------------------------
//!	\brief	Set count
//-----------------------------------------------------------------
template <class T>	udword NArray<T>::SetCount(udword c)
{
	if (c>=mSize) {
		mBuffer = (T*)realloc(mBuffer, sizeof(T)* (c + mGrow));
		mSize = c + mGrow;
	}
	return mCount = c;
}


//-----------------------------------------------------------------
//!	\brief	Set size
//-----------------------------------------------------------------
template <class T>	void NArray<T>::SetSize(udword s)
{
	mBuffer = (T*)realloc(mBuffer, sizeof(T)* s);
	mSize = s;
}


//-----------------------------------------------------------------
//!	\brief	Sorts the array using the compare function
//-----------------------------------------------------------------
template <class T>	void NArray<T>::Sort(CompareFnc cmp)
{
	//Quicksort algorithm
	qsort(mBuffer, mCount, sizeof(T), cmp);
}

//-----------------------------------------------------------------
//!	\brief	Search an item in array
//-----------------------------------------------------------------
template <class T>	udword NArray<T>::Find(T& item)
{
	for (udword i=0; i<mCount; i++)
	{
		if (mBuffer[i] == item)
			return i;	//Found
	}

	return -1;	//Not found
}


//-----------------------------------------------------------------
//!	\brief	Add an item at index into the array
//-----------------------------------------------------------------
template <class T>	udword NArray<T>::InsertItem(udword idx, T& item)
{
	if (idx>=mSize)
	{
		mSize=idx+mGrow;
		mBuffer = (T*)realloc(mBuffer, sizeof(T)*mSize);
	}

	T* pBuffer = mBuffer+idx;

	CopyMemory(pBuffer+1, pBuffer, (mSize-idx) * sizeof(T));

	mBuffer[idx] = item;
	return mCount++;
}