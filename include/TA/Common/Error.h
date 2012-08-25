//---------------------------------------------------------------------------------
// File Name: Error.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------


#ifndef TA_ERROR_H
#define TA_ERROR_H

#ifndef TA_DEBUG_H
#include "Debug.h"
#endif 

#ifndef TA_TYPES_H
#include "Types.h"
#endif 



namespace TA
{


#if defined(_DEBUG)
#define TA_ERROR_REPORT_SEVERE(szString) do { TA_ASSERT(0); Error::Report(Error::LEVEL_SEVERE, szString); } while (0)
#define TA_ERROR_REPORT_HARMLESS(szString) do { TA_ASSERT(0); Error::Report(Error::LEVEL_HARMLESS, szString); } while (0)
#else
#define TA_ERROR_REPORT_SEVERE(szString) Error::Report(Error::LEVEL_SEVERE, szString)
#define TA_ERROR_REPORT_HARMLESS(szString) Error::Report(Error::LEVEL_HARMLESS, szString)
#endif


class Error
{
public:

	enum Level
	{
		LEVEL_HARMLESS = 0,
		LEVEL_SEVERE
	};
	static void TAC_CALL SetErrorReportLevel(Level eLevel);
	static Level TAC_CALL GetErrorReportLevel();

	static void TAC_CALL Report(Level eLevel, const Char* szString);


private:
	static Level lllOOlOOOl;
};

inline Error::Level TAC_CALL Error::GetErrorReportLevel() { return lllOOlOOOl; }

}

#endif 
