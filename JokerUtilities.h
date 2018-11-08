/*	***************************************************************************	PROJECT:	JokerUtilities		FILE:		JokerUtilities.h		PURPOSE:	Some useful definitions for for Joker.			COPYRIGHT:	(C) Copyright 1999 by M. Uli Kusterer, all rights reserved.				Subject to the FreeCard GPL derivate, see				http://freecard.sourceforge.net for details.					REACH ME AT:				E-MAIL:		witness@weblayout.com				URL:		http://www.weblayout.com/witness			REVISIONS:		2000-12-10	UK		Added more std:: qualifiers.		1999-02-14	UK		Created.					************************************************************************ */#ifndef OPENCARD_UTILITIES_H#define OPENCARD_UTILITIES_H#pragma mark [Headers]/* --------------------------------------------------------------------------------	Headers:   ----------------------------------------------------------------------------- */#include	<stdlib.h>#include	<ctype.h>#include	<string.h>#include	<stdexcept>#include	<string>#include	<iostream.h>#include	<stdio.h>#pragma mark [Compiler flags]/* --------------------------------------------------------------------------------	Standard Constants:   ----------------------------------------------------------------------------- *//* Shouldn't these be part of MSL??? */#ifndef TRUE#define TRUE 1#endif#ifndef FALSE#define FALSE 0#endif/* --------------------------------------------------------------------------------	Compiler flags:   ----------------------------------------------------------------------------- */#define	DEBUG			TRUE		// If this is true, failed assertions will throw exceptions./* --------------------------------------------------------------------------------	Compiler Workarounds:   ----------------------------------------------------------------------------- */// CodeWarrior:#if defined(__MWERKS__)  #if (__MWERKS__ < 0x0900)    #define MACINTOSH		1  #else    #if macintosh      #define MACINTOSH		1    #endif  #endif#endif#ifdef __MRC__// MPW is always Macintosh#define MACINTOSH		1typedef int bool;// MPW string class:namespace "std" {class string{protected:	char*		cstr;public:	string() { str = NULL; };	string(const char * str ) { str = cstr; };	virtual const char* c_str() const {return cstr;};};inline runtime_error::runtime_error(const string &) { }}#endif/* --------------------------------------------------------------------------------	Namespace stuff:   ----------------------------------------------------------------------------- */// Define some assert Macros:#ifdef DEBUG	#define ASSERT_Range(x) \        if (!(x)) throw std::out_of_range( "Out of range exception occurred\n" );	#define ASSERT_Argument(x) \        if (!(x)) throw std::invalid_argument( "Invalid argument passed\n" );#else	#define ASSERT_Range(x)	#define ASSERT_Argument(x)#endif// Define a utility macro for specifying our exceptions:#define DEFINE_EXCEPTION( XCEPT_NAME, XCEPT_MESSAGE )	\class XCEPT_NAME : public std::runtime_error \{ \public: \	XCEPT_NAME() : std::runtime_error( XCEPT_MESSAGE ) {}; \};// Exception class that provides function & file name of error:class InternalError : public std::runtime_error{protected:	std::string		mFunctionName;	std::string		mFileName;public:	InternalError( const std::string& fle, const std::string& fcn, const std::string& msg )		: std::runtime_error( msg ),		mFunctionName(fcn), mFileName(fle) {};};// Exception class that provides function & file name of error:class MemoryError : public std::bad_alloc{protected:	std::string		mFunctionName;	std::string		mFileName;	std::string		mMessage;public:	MemoryError( const std::string& fle, const std::string& fcn, const std::string& msg )		: std::bad_alloc(),		mFunctionName(fcn), mFileName(fle), mMessage(msg) {};	virtual const char* what() const { return mMessage.c_str(); }};#endif /* OPENCARD_UTILITIES_H */