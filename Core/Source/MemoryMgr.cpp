//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MemoryMgr.cpp
//! \brief	Memory manager for FxGen
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com) (based on nvidia sdk)
//!	\date		03-09-2008
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
//                   Includes
//-----------------------------------------------------------------
#include "../Include/CoreLib.h"
#include "MemoryMgr.h"

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------

#ifdef TOTO
NDebugMemoryMgr gMemMgr;


void* NDebugMemoryMgr::Malloc(udword size, const char* file, udword line)
{

  //Alloc block
  MemBlk* blk = (MemBlk*)malloc(size + sizeof(MemBlk));

  //Fill MemBlock Struct
  blk->size	= size;
  blk->line	= line;
  blk->file	= file;

  //Link Block
  if (!mFirstBlk)
  {
   	mFirstBlk=mLastBlk=blk; blk->pPrevBlk=blk->pNextBlk=null;
  } else {
    blk->pPrevBlk=mLastBlk;
		blk->pNextBlk=null;
		mLastBlk->pNextBlk=blk;
		mLastBlk=blk;
  }

  //Update Total used mem
  mUsedMem+=size;

  //Update Memory Peak
  if (mUsedMem>mPeakMem)	mPeakMem = mUsedMem;

  return blk + 1;
}


void NDebugMemoryMgr::Free(void* mem)
{
  //Get MemBlk struture
  MemBlk* blk = ((MemBlk*)mem) - 1;

	if (mFirstBlk==blk)		mFirstBlk=blk->pNextBlk;
	if (mLastBlk==blk)		mLastBlk=blk->pPrevBlk;

  //Unlink block
	if (blk->pPrevBlk)
		blk->pPrevBlk->pNextBlk=blk->pNextBlk;

  if (blk->pNextBlk)
    blk->pNextBlk->pPrevBlk=blk->pPrevBlk;

  //Update Total used mem
  mUsedMem-=blk->size;

  //Free Block
  //free(blk);

}


/*void* NDebugMemoryMgr::Realloc(void* mem, udword size, const char* file, udword line)
{

  //Get MemBlk struture
  MemBlk* blk = (MemBlk*) mem - 1;

  //Alloc block
  MemBlk* newblk = (MemBlk*)malloc(size + sizeof(MemBlk));

  //Copy old MemBlk to new MemBlk
  NMemCopy(newblk, blk, sizeof(MemBlk) + min(blk->size, size) );

  //Update Total used mem
  mUsedMem = mUsedMem - blk->size + size;

  //Update Memory Peak
  if (mUsedMem>mPeakMem)	mPeakMem = mUsedMem;

  //Free old Block
  free(blk);

  //Update MemBlk
  newblk->size	= size;
  newblk->line	= line;
  newblk->file	= file;
  return newblk + 1;
}*/


void NDebugMemoryMgr::DumpAllocs()
{
  for (MemBlk* blk = mFirstBlk; blk; blk=blk->pNextBlk)
  {
    TRACE("(WARN) : Memory Leak %dBytes L%d <%s>\n", blk->size, blk->line, blk->file );
    //char Temp[256];
		//strncpy( Temp, (char*)(blk+1), min(255, blk->size) );
    //TRACE("(WARN) :Memory Leak %d %s <%s>", blk->size, blk->file, Temp);
  }

  TRACE("(INFO) : Total Memory Leaks %d Bytes\n",	mUsedMem );
  TRACE("(INFO) : Peak Memory %d Bytes\n", mPeakMem );
}


#endif
