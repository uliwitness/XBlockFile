/*	***************************************************************************	PROJECT:	Joker		FILE:		XEndianFile.h		PURPOSE:	A file class that performs endian conversion.			COPYRIGHT:	(C) Copyright 1999 by M. Uli Kusterer, all rights reserved.				Subject to the FreeCard GPL derivate, see				http://freecard.sourceforge.net for details.					REACH ME AT:				E-MAIL:		witness@weblayout.com				URL:		http://www.weblayout.com/witness			REVISIONS:		1999-07-19	UK		Created.					************************************************************************ */// The following preprocessor directive ensures this file isn't included multiple times:#ifndef X_ENDIAN_FILE_H#define X_ENDIAN_FILE_H#pragma mark [Headers]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Headers:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */   #include	"JokerUtilities.h"#if MACINTOSH#include	<Types.h>#endif#pragma mark [Constants]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Constants:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */#define MAX_FILENAME_SIZE			256			// Maximum size allocated for storing a file's name.#ifndef DATA_COPY_CHUNK_SIZE#define DATA_COPY_CHUNK_SIZE		16384		// Copy in 16k blocks.#endif/* Struct field types for streaming structs to disk:	The constant for a packed size is always (unpacked & ~1). */enum SftType{	SFT_END			= -1,	SFT_PACKED_CHAR	= 0,	SFT_CHAR,				// In struct.	SFT_PACKED_SHORT,	SFT_SHORT,				// In struct.	SFT_PACKED_LONG,	SFT_LONG				// In struct.};#define	SFT_BOOLEAN			SFT_CHAR#pragma mark [Data Structures]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Data Structures:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */typedef enum SftType	SftType;#pragma mark [Class Declaration]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Class declaration:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */class	XEndianFile{protected:	FILE*					mFile;							// The file when opened.	char					mFileName[MAX_FILENAME_SIZE];	// Name of the file.	bool					mEndianSwapped;					// TRUE if we need to perform endian-conversion.	size_t					mCacheStart;					// Start of pre-read data.	size_t					mCacheLength;					// Length of pre-read data.	void*					mCacheData;						// Pointer to pre-read data.  #if MACINTOSH	OSType					mFileType;						// File creator for Macs.	OSType					mFileCreator;					// File type for Macs.  #endifpublic:	XEndianFile();	XEndianFile( char* fName );	virtual ~XEndianFile();		bool			Exists();	virtual void	Open( bool allowWrite );	// Creates if it doesn't exist and write access was requested.	virtual void	Close();	void			Rename( char* inFileName );	void			Kill();		size_t			GetLength();	void			SetLength( size_t inSize )	{};	// ANSI does this automatically. But call this so your code compiles x-platform.	FILE*			GetFile()					{ return mFile; };	FILE*			StealFile()					{ FILE* f = mFile; mFile = NULL; return f; };	// Steal this file's FILE*.	void			CopyBetweenFiles( FILE* srcFile, FILE* dstFile, size_t srcOffs,										size_t dstOffs, size_t length );		void			SetOffset( size_t pOffs );	void			MoveOffset( size_t pOffs );	size_t			GetOffset();		void			Write( void* inData, size_t inLength );	void			Read( void* ioData, size_t inLength );		long			ReadLong();	short			ReadShort();	char			ReadChar();	void			ReadStruct( void* ioData, SftType inTemplate[] );		void			WriteLong( long pNumber );	void			WriteShort( short pNumber );	void			WriteChar( char pNumber );	void			WriteStruct( void* inData, SftType inTemplate[] );		size_t			GetStructSize( SftType inTemplate[] );	void			SetEndianSwap( bool inMode )	{ mEndianSwapped = inMode; };	bool			GetEndianSwap()					{ return mEndianSwapped; };		/* The following don't rename, they manipulate the search path.		For renaming, use Rename() above. */	void			SetFileName( char* inFileName )			{ strcpy( mFileName, inFileName ); };	void			GetFileName( char* outFileName )		{ strcpy( outFileName, mFileName ); };		/* This function allows writing cross-platform code w/o needing to check for Macintosh.		Just call myFile.SetFileType( 'TYPE', 'CREA' ); and your files will have that type on Mac		or this will become a no-op under Linux, Windows etc.				Please only use constants as parameters to this call. This way, we can conditional-compile		these constants in case some other platform except Mac needs a similar call. */  #if MACINTOSH	void			SetFileType( long inType, long inCreator )	{ mFileType = (OSType) inType; mFileCreator = (OSType) inCreator; };  #else	void			SetFileType( long inType, long inCreator )	{};  #endif};#endif /* X_ENDIAN_FILE_H */