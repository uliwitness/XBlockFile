/*	XBFTester		(c) Copyright 2000 by M. Uli Kusterer, all rights reserved. This program and its	source code is subject to the FreeCard GPL derivate, see	http://freecard.sourceforge.net for details.		This is a little testing program with only a CLI that allows applying the	different features of the XBlockFile and XEndianFile classes. It has some	inherent limits, but those are only in the testing program, not in the	classes shipped along.*/#include	"XBlockFile.h"#include	"XEndianFile.h"#include	<LowMem.h>#include	<vector>#include	<map>// A structure that may be used for XEndianFile.struct DataLayoutType{	char		mChar;	short		mShort;	long		mLong;};// An additional structure that tells XEndianFile how the struct is stored in RAM:static SftType		DataLayout[] = { SFT_CHAR, SFT_SHORT, SFT_LONG, SFT_END };// Prototypes:int	XBFDemo();int	XEFDemo();int	main(){	//XEFDemo();	// Test XEndianFile.	XBFDemo();		// Test XBlockFile.		return 0;}int	XEFDemo(){	XEndianFile		vFile( "TestFile.edn" );	DataLayoutType	s;		vFile.SetEndianSwap( TRUE );	vFile.Open(true);		cout << "Enter a char: ";	cin >> s.mChar;	cout << "Enter a short: ";	cin >> s.mShort;	cout << "Enter a value in long range: ";	cin >> s.mLong;		cout << "Size of whole struct: " << sizeof(s) << endl;	cout << "Struct data: " << s.mChar << ", " << s.mShort << ", " << s.mLong << endl;		vFile.SetOffset( 0 );	vFile.WriteStruct( &s, DataLayout );		vFile.SetOffset( 0 );	vFile.ReadStruct( &s, DataLayout );		cout << "Read data: " << s.mChar << ", " << s.mShort << ", " << s.mLong << endl << endl;		vFile.Close();		return 0;}/* A busy proc. XBlockFile will call back to this function, passing	a constant indicating what is currently happening and how much it has	progressed. You can use this to bring up a progress bar or spin an	animated cursor to provide user feedback. */void	XBFBusyProcConsole( unsigned long busyAction, XBlockOffset inMax, XBlockOffset inCurr ){	switch( busyAction )	{		case BUSY_ACTION_LOAD:			cout << "Loading block ";			break;				case BUSY_ACTION_LOAD_WASTED:			cout << "Loading wasted entry ";			break;				case BUSY_ACTION_SAVE_DATA:			cout << "Saving data of block ";			break;				case BUSY_ACTION_SAVE_MAP:			cout << "Saving block ";			break;				case BUSY_ACTION_SAVE_WASTED:			cout << "Saving wasted entry ";			break;	}		cout << inCurr << " of " << inMax << "...\n";}int	XBFDemo(){		char			vBufferOne[256] = "",					vBufferTwo[256] = "";	XBlockFile		vFile( "TestFile.ocd" );	bool			vQuit = FALSE;	XBlockEntry*	vCurrBlock = NULL;		cout << "----------------------------------------------------------------------\n";	cout << " XBlockFile Demo 1.0\n";	cout << " Created by M. Uli Kusterer\n\n";	cout << " WARNING:\n";	cout << "	The buffers in this block file are all fixed-size and limited to\n";	cout << "	255 characters (=bytes!). XBlockFile supports longer blocks but\n";	cout << "	this demo will choke on them on input/output. Don't do it!\n\n";	cout << " NOTE:\n";	cout << "	ANSI's routines for user input currently stop at whitespace, so\n";	cout << "	please enter no spaces or tabs if you are asked to specify block\n";	cout << "	data. Again, this is a limit of this demo, not of XBlockFile.\n\n";	cout << " REVISION:\n";	cout << "   2000-02-19	UK	Finished rewrite of XBF for sub-blocks.\n";	cout << "	1999-05-25	UK	Added deletion of blocks API.\n";	cout << "	1999-03-31	UK	Initial release.\n";	cout << "----------------------------------------------------------------------\n\n";	cout << "Creating/opening file \"TestFile.ocd\"...\n";		vFile.SetBusyProc( XBFBusyProcConsole );	// Install our feedback proc.		// Open the file and time that. FIX ME! Shouldn't use MacOS timing calls, should use ANSI!	long		vtime, vendTime;	vtime = LMGetTicks();	vFile.Open(true);	vendTime = LMGetTicks();		cout << "This took " << vendTime -vtime << " ticks.\n\n";		cout << "Enter HELP for a list of commands.\n\n";		/* Now enter a loop where we keep asking the user for actions: */	while( !vQuit )	{		short		x;				cout << "\nWhat do you want to do? ";		cin >> vBufferOne;		cout << endl;				// Make string all lower-case:		for( x = 0; vBufferOne[x] != 0; x++ )			vBufferOne[x] = tolower( vBufferOne[x] );				try {			// Determine what command it is:			if( strcmp( vBufferOne, "quit" ) == 0 )				vQuit = true;			else if( strcmp( vBufferOne, "help" ) == 0 )			{				cout << "Enter one of the uppercase keywords:\n";				cout << "WRITE a new root block to the file.\n";				cout << "READ a root block from the file.\n";				cout << "set the CURRENT block\n";				cout << "ADD a new sub block to the current block\n";				cout << "make a SUB block of the current block current\n";				cout << "SET the contents of the current block\n";				cout << "GET the current block's contents\n";				cout << "DELETE the current block\n";				cout << "COMPACT the file\n";				cout << "IMPORT a file into a block\n";				cout << "EXPORT a block to a file\n";				cout << "STREAM some data to the current block\n";				cout << "add a HUGE number of blocks (50 000) w/o feedback\n";				cout << "add a BIG number of blocks (20 000).\n";				cout << "BIGFAST like \"big\" but w/o feedback.\n";				cout << "DUMP the file's block map to the screen.\n";				cout << "QUIT (to close this program).\n";				cout << "HELP to display this list.\n\n";			}			else if( strcmp( vBufferOne, "read" ) == 0 )			{				long		vID;				char*		vString;				long		time,							endTime;								cout << "Read an existing block from the block file.\n";								cout << "Enter ID (integer) of block: ";				cin >> vID;				cout << "\nReading block...\n";				time = LMGetTicks();				vString = (char*) vFile.GetBlock( vID );				endTime = LMGetTicks();				cout << "The data is: " << vString << endl;								cout << "This took " << endTime -time << " ticks." << endl << endl;			}			else if( strcmp( vBufferOne, "stream" ) == 0 )			{				size_t		vOffset;								cout << "Stream data to the current block.\n";								cout << "\nAt what offset do you want the data to be written? ";				cin >> vOffset;				cout << "\nWhat is the data you want to write? ";				cin >> vBufferTwo;								vFile.StreamToBlockEntry( vCurrBlock, vBufferTwo, strlen(vBufferTwo),											vOffset );								cout << "Successfully streamed.\n";			}			else if( strcmp( vBufferOne, "import" ) == 0 )			{				long		vID;				char		vString[256];				FILE*		vDataFile;								cout << "Import a text file into a block.\n";				cout << "Note this file needs to be zero-terminated or this demo will output too much data.\n\n";								cout << "Enter ID (integer) of block: ";				cin >> vID;				cout << "\nEnter the name of the file:\n";				cin >> vString;				cout << "\nImporting block...\n";				vDataFile = fopen( vString, "r+" );				if( vDataFile )				{					vFile.SetBlockFromFile( vID, vDataFile );					fclose( vDataFile );										cout << "Import successful.\n";				}				else					cout << "Error.\n";			}			else if( strcmp( vBufferOne, "export" ) == 0 )			{				long		vID;				char		vString[256];				FILE*		vDataFile;								cout << "Export a block to a text file.\n";								cout << "Enter ID (integer) of block: ";				cin >> vID;				cout << "\nEnter the name of the file:\n";				cin >> vString;				cout << "\nExporting block...\n";				vDataFile = fopen( vString, "w" );				if( vDataFile )				{					vFile.GetBlockToFile( vID, vDataFile );					fclose( vDataFile );										cout << "Export successful.\n";				}				else					cout << "Error.\n";			}			else if( strcmp( vBufferOne, "delete" ) == 0 )			{				long		vID;								cout << "Delete an existing block of type \"TEXT\" from the block file.\n\n";								cout << "Enter ID (integer) of block: ";				cin >> vID;				vFile.KillBlock( vID );				cout << "Block deleted.\n\n";			}			else if( strcmp( vBufferOne, "add" ) == 0 )			{				long		vID,							time,							endTime;				int			vCharacter = 0;				short		x = 0;								cout << "Add a new block to the current block.\n";				cout << "This will overwrite any existing block of same ID!\n\n";								cout << "Enter ID (integer) for block: ";				cin >> vID;				cout << "\nEnter Text (data) for the block: ";				cin >> vBufferTwo;				cout << "\nWriting block...\n";				time = LMGetTicks();				vFile.AddSubBlock( vCurrBlock, vID, vBufferTwo, strlen(vBufferTwo) +1 );				endTime = LMGetTicks();				cout << "Block written.\n";				cout << "This took " << endTime -time << " ticks." << endl << endl;			}			else if( strcmp( vBufferOne, "write" ) == 0 )			{				long		vID,							time,							endTime;				int			vCharacter = 0;				short		x = 0;								cout << "Write a new block to the block file.\n";				cout << "This will overwrite any existing block of same ID!\n\n";								cout << "Enter ID (integer) for block: ";				cin >> vID;				cout << "\nEnter Text (data) for the block: ";				cin >> vBufferTwo;				cout << "\nWriting block...\n";				time = LMGetTicks();				vFile.SetBlock( vID, vBufferTwo, strlen(vBufferTwo) +1 );				endTime = LMGetTicks();				cout << "Block written.\n";				cout << "This took " << endTime -time << " ticks." << endl << endl;			}			else if( strcmp( vBufferOne, "bigfast" ) == 0 )			{				short		x = 0;				long		time,							endTime;				char		vData[] = "This Is The Data I write to a block";								cout << "Writing 20 000 blocks...\n";								time = LMGetTicks();								while( ++x < 20001 )					vFile.SetBlock( x +499, vData, strlen(vData) +1 );								endTime = LMGetTicks();								cout << "This took " << endTime -time << " ticks." << endl;				cout << "This is " << (endTime -time) / 20000 << " for 1 block." << endl;			}			else if( strcmp( vBufferOne, "big" ) == 0 )			{				short		x = 0;				long		time,							endTime;				char		vData[] = "This Is The Data I write to a block";								cout << "Writing 20 000 blocks...\n";								time = LMGetTicks();								while( ++x < 20001 )				{					vFile.SetBlock( x +499, vData, strlen(vData) +1 );					if( (x % 100) == 0 )						cout << x << " blocks written.\n";				}								endTime = LMGetTicks();								cout << "This took " << endTime -time << " ticks." << endl;				cout << "This is " << (endTime -time) / 20000 << " for 1 block." << endl;			}			else if( strcmp( vBufferOne, "huge" ) == 0 )			{				short		x = 0;				long		time,							endTime;				char		vData[] = "This Is The Data I write to a block";								cout << "Writing 50 000 blocks...\n";								time = LMGetTicks();								while( ++x < 50001 )					vFile.SetBlock( x +499, vData, strlen(vData) +1 );								endTime = LMGetTicks();								cout << "This took " << endTime -time << " ticks." << endl;				cout << "This is " << (endTime -time) / 20000 << " for 1 block." << endl;			}			else if( strcmp( vBufferOne, "compact" ) == 0 )			{				long		time,							endTime;								time = LMGetTicks();								cout << "Beginning compaction...\n";				vFile.Compact();				endTime = LMGetTicks();				cout << "Compaction finished.\n\n";								cout << "This took " << endTime -time << " ticks." << endl;			}			else if( strcmp( vBufferOne, "current" ) == 0 )			{				long		vID;								cout << "Enter ID of block to make current: ";				cin >> vID;				vCurrBlock = vFile.GetBlockEntry( vID );			}			else if( strcmp( vBufferOne, "sub" ) == 0 )			{				long		vID;								cout << "Enter ID for sub-block to make current block: ";				cin >> vID;				vCurrBlock = vCurrBlock->GetBlockEntry( vID );			}			else if( strcmp( vBufferOne, "get" ) == 0 )			{				char*		vString;				long		time,							endTime;								cout << "\nReading block...\n";				time = LMGetTicks();				vString = (char*) vFile.GetBlockData( vCurrBlock );				endTime = LMGetTicks();				cout << "The data is: " << vString << endl;				// Don't dispose! Belongs to block file!								cout << "This took " << endTime -time << " ticks." << endl << endl;			}			else if( strcmp( vBufferOne, "dump" ) == 0 )			{				vFile.DumpMap();			}		}		catch( exception &theErr )		{			cout << "An Error occurred:" << theErr.what() << endl;		}				vBufferOne[0] = 0;	// Clear string.	}		cout << "Quitting...\n";	vtime = LMGetTicks();	vFile.Close();	vendTime = LMGetTicks();	cout << "This took " << vendTime -vtime << " ticks." << endl;		cout << "You might have to choose \"Quit\" from the \"File\" menu to quit the console window.";		return 0;}