/*	***************************************************************************	PROJECT:	Joker		FILE:		XBlockFile.h		PURPOSE:	A block file for storing bits of information as random-accessible blocks.			COPYRIGHT:	(C) Copyright 1999-2000 by M. Uli Kusterer, all rights reserved.				Subject to the FreeCard GPL derivate, see				http://freecard.sourceforge.net for details.					REACH ME AT:				E-MAIL:		witness@weblayout.com				URL:		http://www.weblayout.com/witness			REVISIONS:		1999-03-24	UK		Created.					************************************************************************ */// Make sure this is included only once:#ifndef X_BLOCK_FILE_H#define X_BLOCK_FILE_H#pragma mark [Headers]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Headers:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */   #include	"JokerUtilities.h"#include	"XEndianFile.h"#include	"XBusyProc.h"#include	<map>#pragma mark [Compiler switches]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Compiler switches:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */// This dumps the file's map in text form to the console on opening, and on//  closing before and after saving the file.#ifndef DEBUG_DUMP_MAP_HIERARCHY#define		DEBUG_DUMP_MAP_HIERARCHY	1#endif// This allows you to set "breakpoints" on a certain block by setting a//  flag in the struct using the debugger.#ifndef DEBUG_SUPPORT_BREAKPOINTS#define		DEBUG_SUPPORT_BREAKPOINTS	1#endif// This causes XBlockFile to detect basic block file corruption by checking//  for collisions of block offsets with the file header.#ifndef DEBUG_CHECK_VALID_OFFSETS#define		DEBUG_CHECK_VALID_OFFSETS	1#endif#pragma mark [Data Structures]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Data Structures:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */class	XBlockFile;	// Forward.// ID of a block, a positive number:typedef	unsigned long	XBlockID;// Offset in file:typedef unsigned long	XBlockOffset;// Flags data type:typedef unsigned char	XBlockFlags;/* To work around recursive definitions, we define a void pointer in XBlockEntry	into which we'll put an XBlockMap object which we create and delete in the	con-/destructor.  */// Each entry in the file's map is of this type:class XBlockEntry{public:	XBlockID						mId;			// Index of block in map.	XBlockOffset					mOffset;		// Start of block data in file.	XBlockOffset					mLength;		// Physical size of block data in file.	XBlockOffset					mLogLength;		// Used size of block data in file.	//XBlockOffset					mSubMapLength;	// <Obsolete>	void*							mPointer;		// Pointer to loaded block data (malloc! NOT new!).	XBlockOffset					mPointerLength;	// Length of data in RAM.	XBlockFlags						mFlags;			// Some flags for this entry.	void*							mSubBlocks;		// List of sub-blocks of this block. This holds an XBlockMap*.	XBlockFile*						mFile;			// The file this block belongs to.	XBlockEntry*					mSuperEntry;	// The entry this is a sub-entry of. NULL if root entry.	void*							mRefCon;		// Storage for user data.  #if DEBUG_SUPPORT_BREAKPOINTS	bool							mBreakOnThis;	// TRUE to cause a breakpoint on certain actions on this block.  #endif /*DEBUG_SUPPORT_BREAKPOINTS*/	// If you add a data member, be sure to fix XBlockFile::DumpMap() to output that, too!		XBlockEntry( XBlockFile* inFile = NULL, XBlockEntry* superEntry = NULL );	XBlockEntry( const XBlockEntry& inEntry );	// Copy constructor.	~XBlockEntry();		void			SwapData( XBlockEntry& ioBlock );	void			UnloadData()	{ if( mPointer ) { free( mPointer ); mPointer = NULL; mPointerLength = 0; }; };		XBlockEntry&	operator=(const XBlockEntry& inEntry);		XBlockEntry*	GetBlockEntry( XBlockID inID );					// Get a sub block of this one by ID.	XBlockEntry*	GetIndBlockEntry( unsigned long inNum );		// Get a sub block of this one by number (=index).	unsigned long	CountSubBlocks();	XBlockFile*		GetOwningFile()									{ return mFile; };		size_t			GetLength();	void			SetLength( size_t l );	XBlockOffset	GetOffset();	void			SmashOffset( XBlockOffset offs );		void			SetDirty();		// Call this when you have changed the block's data.		void			WriteToFile( XEndianFile& destFile );		void			SetRefCon( void* d )	{ mRefCon = d; };	void*			GetRefCon()				{ return mRefCon; };protected:	void	ModelMeAfter( const XBlockEntry& inEntry );};// Size of block entry in RAM:#define BLOCK_ENTRY_SIZE		(sizeof(XBlockID) +(sizeof(XBlockOffset) *3) +sizeof(XBlockFlags) +(sizeof(void*) *2))// Size of block entry on disk:#define	DISK_BLOCK_ENTRY_SIZE	(sizeof(XBlockID) +(sizeof(XBlockOffset) *2) +sizeof(XBlockFlags))class XBlockWastedSpace{public:	XBlockOffset	mOffset;	XBlockOffset	mLength;};// Size of fields in above class so XEndianFile can convert it:static SftType	XBFWastedSpaceTmpl[] = { SFT_LONG, SFT_LONG, SFT_END };// Size of wasted entry on disk:#define DISK_WASTE_ENTRY_SIZE	(sizeof(XBlockOffset) *2)// Some convenient names to use instead of these huge C++ ones:typedef std::multimap<XBlockOffset,XBlockWastedSpace>	XWastedBlockList;typedef XWastedBlockList::iterator						XWastedIterator;typedef XWastedBlockList::value_type					XWastedListItem;typedef std::map<XBlockID,XBlockEntry>					XBlockMap;typedef	XBlockMap::iterator								XBlockMapIterator;#pragma mark [Constants]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Constants:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */enum{	// File version that indicates format:	BLOCK_FILE_VERSION					= 0,	// This is stored big-endian *always*!		// Size of data before block data begins:	BLOCK_FILE_HEADER_FILEEND_OFFSET	= (sizeof(unsigned short) +sizeof(XBlockFlags)),				// This is where mFileEnd is saved.	BLOCK_FILE_HEADER_MAPOFFS_OFFSET	= (BLOCK_FILE_HEADER_FILEEND_OFFSET +sizeof(XBlockOffset)),		// Here mMapOffset is saved.	BLOCK_FILE_HEADER_SIZE				= (BLOCK_FILE_HEADER_MAPOFFS_OFFSET +sizeof(XBlockOffset)),		// Here block data starts.		// Amount of bytes to add to physical size of a block when we run out of space during streaming:	AUTO_FLUFF_AMOUNT					= 100};/* Constants used to assign file types on Mac. Other platforms probably don't	need this as they append that information to file names, but always	remember to call XEndianFile's SetFileType() method with the appropriate	constants for all non-temporary files. */enum{#if COMPILING_SAL#if MACINTOSH	BLOCK_FILE_TYPE		= 'JSTK',	// Type of block file.	BLOCK_FILE_CREATOR	= 'JOKE'	// Creator code of Joker.#else	BLOCK_FILE_TYPE		= 1,		// In case we later need to distinguish file types, use a different number here.	BLOCK_FILE_CREATOR	= 0			// Zero means Joker itself.#endif#else#if MACINTOSH	BLOCK_FILE_TYPE		= 'BLOK',	// Type of block file.	BLOCK_FILE_CREATOR	= 'FREE'	// Creator code of Joker.#else	BLOCK_FILE_TYPE		= 1,		// In case we later need to distinguish file types, use a different number here.	BLOCK_FILE_CREATOR	= 0			// Zero means Joker itself.#endif#endif};// File header flags:enum{	FILE_FLAG_BIG_ENDIAN		= (1 << 0)	// Numbers etc. were written Mac-endian.};// Block flags:enum{	// Block data in RAM differs from on-disk version (cleared when saved to disk).	BLOCK_DIRTY					= (1 << 0),	// Block data hasn't been saved to disk yet. (cleared when saved to disk) WARNING: Offset & length are invalid!	BLOCK_RAM_ONLY				= (1 << 1),	// Block may be unloaded from RAM when it is saved or more memory is needed and it isn't dirty.	BLOCK_PURGEABLE				= (1 << 2),	/* This block contains a map of its sub-blocks. Any data storage/retrieval is		re-routed to sub-block 0. The map may be loaded into mSubBlocks. */	BLOCK_HAS_SUB				= (1 << 3),	/* All flags that are not supposed to be saved to disk must be added to		this mask. The inverse of this will be used to mask out these flags		during saving and compacting. */	VOLATILE_BLOCK_FLAGS_MASK	= (BLOCK_DIRTY | BLOCK_RAM_ONLY)};#pragma mark [Class Declaration]/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ	Class declaration:   ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */class	XBlockFile : public XEndianFile{friend class XBlockEntry;protected:	bool				mFileChanged;		// TRUE if map needs to be written again. If XBlockFile changes anything, it sets this!!!	XBlockMap			mBlockMap;			// Our map of blocks containing actual data.	XWastedBlockList	mWastedBlocks;		// List of blocks ready for re-use.	XWastedBlockList	mNewlyWastedBlocks;	// List of blocks we'll re-use after the next successful save.	XBlockOffset		mMapOffset;			// Offset of map in file.	XBlockOffset		mMapLength;			// Length of map in file.	XBusyProcPtr		mBusyProc;			// Procedure to call when this is busy to provide feedback.	XBlockOffset		mFileEnd;			// The place where we'll append new blocks to our file.	public:	XBlockFile();	XBlockFile( const char* fName );	// Preparation/cleanup:	virtual void	Open( bool allowWrite );	virtual void	Close();	void			Compact();	void			SaveFile();	void			WriteEmptyBlockFile();	// Manipulating root-level blocks (shortcut methods):	void*			GetBlock( XBlockID inID );	XBlockEntry*	SetBlock( XBlockID inID, void* inData, XBlockOffset inLength );	void			GetBlockToFile( XBlockID inID, FILE* inFile );	XBlockEntry*	SetBlockFromFile( XBlockID inID, FILE* inFile );	void			KillBlock( XBlockID inID );	void			KillBlockEntry( XBlockEntry* inEntry );	// streaming:	void			StreamToBlockEntry( XBlockEntry *vTheBlock,										void* inData, XBlockOffset inLength,										XBlockOffset inWritePosition );	void			StreamToBlock( XBlockID inID,									void* inData, XBlockOffset inLength,									XBlockOffset inWritePosition );	void			StreamFromBlockEntry( XBlockEntry* vTheBlock,											void* ioData, XBlockOffset inLength,											XBlockOffset inWritePosition );	void			StreamFromBlock( XBlockID inID,										void* ioData, XBlockOffset inLength,										XBlockOffset inWritePosition );	void			FluffBlock( XBlockEntry* inBlock, size_t inAmount );	// Feedback:	void			SetBusyProc( XBusyProcPtr n )	{ mBusyProc = n; };		void			DumpMap( bool doLoad = false )	{ DumpMap( mBlockMap, 0, doLoad ); DumpWastedList(); };	void			DumpWastedList();	void 			DumpMap( XBlockMap &inMap, unsigned long inNestLevel = 0, bool doLoad = false );	// Debugging only!	// Working with blocks:	XBlockEntry*	AddSubBlock( XBlockEntry* inEntry, XBlockID inID,											void* inData, XBlockOffset inLength );	void			ResizeBlockEntry( XBlockEntry* vTheEntry, XBlockOffset inLength );	XBlockEntry*	GetBlockEntry( XBlockID inID );	// Gets entry for root block.	void*			GetBlockData( XBlockEntry* vFoundEntry );	// Pointer is owned by XBF!	XBlockEntry*	GetSubBlockEntry( XBlockEntry* owner, XBlockID inID )	{ return( owner->GetBlockEntry( inID ) ); };	unsigned long	CountSubBlocks( XBlockEntry* owner )					{ return( owner->CountSubBlocks() ); };	XBlockEntry*	GetIndSubBlockEntry( XBlockEntry* owner, unsigned long inNum )	{ return( owner->GetIndBlockEntry( inNum ) ); };	protected:	XBlockEntry*	AddBlockToMap( XBlockMap& inMap, XBlockID inID, void* inData,											XBlockOffset inLength );	XBlockMap*		GetSubBlockMap( XBlockEntry* inEntry );		void			LoadBlockMap();	void			LoadWastedIntoList();	void			LoadBlocksIntoList( XBlockMap& ioList, XBlockEntry* forEntry = NULL );	void			KillFileMap();	XBlockOffset	WriteBlockMap( XBlockMap& vTheMap, XBlockOffset vBlockOffs, XBlockOffset &vFileEnd );	XBlockOffset	StoreOneMapWithoutData( XBlockMap& theMap, XBlockOffset vWritePos,													XEndianFile& destFile );	void			CompactBlockMap( XBlockMap &srcMap, XBlockOffset &vCurrWritePosition,										XEndianFile& tempFile, XBlockMap& dstMap );		void			WasteBlockEntry( XBlockEntry* inEntry )		{ WasteBlockEntry( inEntry, mBlockMap ); };	void			WasteBlockEntry( XBlockEntry* inEntry, XBlockMap& inMap );	XBlockEntry*	SplitBlock( XBlockEntry* inEntry, XBlockOffset inDesiredSize );	void			KillMapEntry( XBlockEntry* inEntry )	{ KillMapEntry( inEntry, mBlockMap ); };	void			KillMapEntry( XBlockEntry* inEntry, XBlockMap& inMap );	XBlockEntry*	MoveBlockToEnd( XBlockEntry* inBlock );		bool			GetWastedBlockEntry( XBlockOffset inDesiredSize, XBlockWastedSpace* outBlockSpace,											bool doDelete = false );	bool			GetExactWastedBlockEntry( XBlockOffset inDesiredSize, XBlockWastedSpace* outBlockSpace,												bool doDelete = false );												XBlockOffset	CalculateFileEnd();};#endif /* X_BLOCK_FILE_H */