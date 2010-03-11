//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CompilerOp.h
//! \brief	Operators Compiler
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		02-07-2009
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef COMPILEROP_H
#define COMPILEROP_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../Include/FxGenLib.h"
#include "Templates.h"
#include "EngineOp.h"

/*#ifdef GetUserName
#undef GetUserName
#endif*/


//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NCompilerOp;
struct SOpCallDesc;
struct SCompiledOp;
struct SOpsSequence;
struct SOpFuncInterface;

//-----------------------------------------------------------------
//!	\struct		SOperatorDesc
//!	\brief		Operators desc for compiler
//-----------------------------------------------------------------
struct SOperatorDesc
{
	udword	x, y, w;			//!< Position x and y in the graph and width
	const char*		pszGroup;			//!< Group name used for incremental compilation (could be texture name)
	const char*		pszIFnc;			//!< Operator fonction interface name
	const char*		pszToLoad;		//!< Operator to Load name
	const char*		pszStoredName;//!< Operator store name
	udword	adwParams[MAX_PARAMS];	//!< Operator parameters
	ubyte		byParamsCount;
	udword	dwTag;
	//Reserved Datas for Compiler
	SOperatorDesc*	m_pnextOpToProcess;
	SOperatorDesc*	m_pprevOpToProcess;
	ubyte m_byDepth;
	ubyte m_byInputs;
};

//-----------------------------------------------------------------
//!	\struct		SLoadOpToSeqPtr
//!	\brief		in order to update sequences toLoad ptr after sequences computing
//-----------------------------------------------------------------
struct SLoadOpToSeqPtr
{
	const char*	pszName;	//!< To Load store operator's name
	//SOpsSequence** ppToLoadStoredSeq;	//!< Ref Target, ptr on	SOpsSequence::apSeqsToLoad
	SOpsSequence*		pLoaderSeq;	//!< Ref Maker
	udword dwLoaderCallDescIdx;
	udword dwLoaderSeqIdx;		//!< Index in SOpsSequence::apSeqsToLoad

};

//-----------------------------------------------------------------
//                   TypesDef
//-----------------------------------------------------------------
typedef NArray<SOperatorDesc*>	NOpsDescArray;

//-----------------------------------------------------------------
//!	\class		NCompilerOp
//!	\brief		Engine operators compiler
//-----------------------------------------------------------------
class FXGEN_API NCompilerOp
{
public:
	//Constructors-Destructor
	NCompilerOp();
	virtual ~NCompilerOp();

	//Methods
	bool Compile(SCompiledOp* _inoutCompOps, SOperatorDesc* _paops, udword _dwCount, bool _bIncremential);
	//Si mode _bIncremential on garde les operateurs compiles sur les groupes differentes
	//  sinon on nettoie tout avant
	//SOpCallDesc* GetOpCallDesc(char* _pszGroup, udword x, udword y);
	bool GetOpCallDescFromTag(SCompiledOp* _pcompOps, udword _dwTag, SOpCallDesc** _opCD, SOpsSequence** _opSeq);

	bool SaveCompiledForm(const char* _pszFullFileName);

protected:
	//Methods
	void ComputeLinks();
	void _ComputeLinks(SOperatorDesc* _pop, SOperatorDesc* _pprevop, udword _dwCurDepth);

	void GetPrevOperators(SOperatorDesc* _pop, NOpsDescArray& _carrayPrevOpS);
	void GetNextOperators(SOperatorDesc* pop, NOpsDescArray& carrayNextOpS);

	void ComputeOpsSequences(SCompiledOp* _inoutCompOps);
	void CompileOperator(SOpsSequence *_pstSeq, SOperatorDesc* _pop);
	bool CompileGraph(SCompiledOp* _inoutCompOps, bool _bOnlyFinalsResults);

	bool RemoveGroup(char *pszName);

	SOperatorDesc* GetFinalOpFrom(SOperatorDesc* pop);

	SOpFuncInterface* GetOpsInterfaceFromName(const char* _pszName);

	//Datas
	NOpsDescArray	m_arrayOpsUnlinked;
	NOpsDescArray	m_arrayOps;
	SOperatorDesc* m_pprevOp;

	NArray<SLoadOpToSeqPtr>	m_arrayLoadToSeq;
};

#endif //COMPILEROP_H

