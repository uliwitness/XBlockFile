/* XBFValidate 1.0	(c) 2000 by M. Uli Kusterer, all rights reserved.	This program and its source code are covered under the	FreeCard GPL variant. See http://freecard.sourceforge.net for details.		This is a little tool that validates files generated by XBlockFile.	However, this only performs some checks on header and main block	map, it doesn't check all sub blocks (you can add that if you need	it). */#include <ios>#include "XEndianFile.h"using namespace std;int main(void){	try	{		XEndianFile		f( "TestFile.ocd" );		size_t			vMapOffs, vActSize, vDesigSize, vMapSize;				f.Open(false);				cout << "Version:         " << f.ReadShort() << endl;		cout << "Flags:           " << f.ReadLong() << endl;		vDesigSize = f.ReadLong();		cout << "Designated Size: " << vDesigSize << endl;		vActSize = f.GetLength();		cout << "Actual Size:     " << vActSize << endl;		vMapOffs = f.ReadLong();		cout << "Map Offset:      " << vMapOffs << endl;		if( vMapOffs <= vActSize -(sizeof(long) *2) )	// Allow at least for an empty map.		{			f.SetOffset( vMapOffs );			size_t vEntryC = f.ReadLong();			cout << "Map Entry count: " << vEntryC << endl;			vMapSize = sizeof(long);			for( size_t x = 1; x <= vEntryC; x++ )			{				size_t	dataSize, blkSize;								vMapSize += 6 *sizeof(long);								if( (vMapOffs +vMapSize) > vActSize )				{					cout << "WARNING: Map extends beyond actual end of file." << endl;					break;				}				if( (vMapOffs +vMapSize) > vDesigSize )				{					cout << "WARNING: Map extends beyond designated end of file." << endl;					break;				}								cout << "\tBlock ID:        " << f.ReadLong() << endl;				cout << "\tData offset:     " << f.ReadLong() << endl;				blkSize = f.ReadLong();				cout << "\tPhysical size:   " << blkSize << endl;				dataSize = f.ReadLong();				cout << "\tLogical size:    " << dataSize << endl;				if( dataSize > blkSize )					cout << "WARNING: Block data extends past block's end." << endl;				cout << "\tFlags:           " << f.ReadLong() << endl;				cout << "\tReserved:        " << f.ReadLong() << endl << endl;			}						// Wasted list:			vEntryC = f.ReadLong();			vMapSize += sizeof(long);			for( size_t y = 1; y <= vEntryC; y++ )			{				vMapSize += 2 *sizeof(long);								if( (vMapOffs +vMapSize) > vActSize )				{					cout << "WARNING: Wasted list of map extends beyond actual end of file." << endl;					break;				}				if( (vMapOffs +vMapSize) > vDesigSize )				{					cout << "WARNING: Wasted list of map extends beyond designated end of file." << endl;					break;				}								cout << "\tWasted Offset:   " << f.ReadLong() << endl;				cout << "\tWasted Length:   " << f.ReadLong() << endl;			}		}		else			cout << "WARNING: Map offset extends beyond end of file." << endl;				if( vActSize != vDesigSize )			cout << "WARNING: Actual and designated sizes differ." << endl;	}	catch( exception& err )	{		cout << endl << err.what() << endl;	}		return 0;}