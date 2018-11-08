/*	***************************************************************************	PROJECT:	XStackFile		FILE:		XStackFile.cpp		PURPOSE:	Stack file class			COPYRIGHT:	(C) Copyright 2000 by M. Uli Kusterer, all rights reserved.					REACH ME AT:				E-MAIL:		witness@weblayout.com				URL:		http://www.weblayout.com/witness		REVISIONS:		2000-07-07	UK		Created.					************************************************************************ */#pragma mark [Headers]/* --------------------------------------------------------------------------------	Headers:   ----------------------------------------------------------------------------- */#include	"XStackFile.h"#pragma mark [Globals]/* --------------------------------------------------------------------------------	Globals/Static Variables:   ----------------------------------------------------------------------------- */// Put these li'l critters here.#pragma mark -#pragma mark [Init/Kill]/* --------------------------------------------------------------------------------	GetStackByID:		Get a reference to a stack which can then be further manipulated.				This first retrieves the stack list block from its block file and then		fetches the appropriate stack block from that block.		TAKES:		theID	-	ID of the stack.		create	-	Set this to TRUE if you want a block created for you when					there is none. Defaults to FALSE.		GIVES:		-		REVISIONS:		2000-10-01	UK		Added 'create' parameter.		2000-07-08	UK		Created.   ----------------------------------------------------------------------------- */XSFStackRef	XStackFile::GetStackByID( XBlockID theID, bool create ){	XBlockEntry*		vStackListBlock;	XSFStackRef			vFoundStack;	XBlockOffset		vDummy = 0;		vStackListBlock = mBlockFile.GetBlockEntry( STACK_LIST_BLOCK_ID );	if( vStackListBlock == NULL )	{		if( create )			vStackListBlock = mBlockFile.SetBlock( STACK_LIST_BLOCK_ID, &vDummy, sizeof(vDummy) );				if( vStackListBlock == NULL )			throw runtime_error( "XStackFile::GetStackByID - block list missing from file." );	}		vFoundStack = vStackListBlock->GetBlockEntry( theID );	if( vFoundStack == NULL && create )	// Doesn't exist yet and we're supposed to create one.		vFoundStack = AddSubBlock( vStackListBlock, theID, &vDummy, sizeof(vDummy) );		return vFoundStack;}/* --------------------------------------------------------------------------------	GetCardByID:		Get a reference to a card which can then be further manipulated.		TAKES:		stack	-	The stack the card belongs to.		theID	-	ID of the card to get.		create	-	Set this to TRUE if you want a block created for you when					there is none. Defaults to FALSE.		GIVES:		-		REVISIONS:		2000-10-01	UK		Added 'create' parameter.		2000-07-08	UK		Created.   ----------------------------------------------------------------------------- */XSFCardRef	XStackFile::GetCardByID( XSFStackRef stack, XBlockID theID, bool create ){	XBlockEntry*		vCardListBlock;	XSFCardRef			vFoundCard;		vCardListBlock = stack->GetBlockEntry( STACK_CARD_LIST_BLOCK_ID );	if( vCardListBlock == NULL )	{		if( create )			vCardListBlock = mBlockFile.AddSubBlock( stack, STACK_CARD_LIST_BLOCK_ID, &vDummy, sizeof(vDummy) );				if( vCardListBlock == NULL )			throw runtime_error( "XStackFile::GetCardByID - block list missing from file." );	}		vFoundCard = vCardListBlock->GetBlockEntry( theID );	if( vFoundCard == NULL && create )		vFoundCard = mBlockFile.AddSubBlock( vCardListBlock, theID, &vDummy, sizeof(vDummy) );		return vFoundCard;}/* --------------------------------------------------------------------------------	GetBkgndByID:		Get a reference to a background which can then be further manipulated.		TAKES:		stack	-	the stack the background belongs to.		theID	-	ID of the background to get.		create	-	Set this to TRUE if you want a block created for you when					there is none. Defaults to FALSE.		GIVES:		-		REVISIONS:		2000-10-01	UK		Added 'create' parameter.		2000-07-08	UK		Created.   ----------------------------------------------------------------------------- */XSFBkgndRef	XStackFile::GetBkgndByID( XSFStackRef stack, XBlockID theID, bool create ){	XBlockEntry*		vBkgndListBlock;	XSFBkgndRef			vFoundBkgnd;		vBkgndListBlock = stack->GetBlockEntry( STACK_BKGND_LIST_BLOCK_ID );	if( vBkgndListBlock == NULL )	{		if( create )			vBkgndListBlock = mBlockFile.AddSubBlock( stack, STACK_BKGND_LIST_BLOCK_ID, &vDummy, sizeof(vDummy) );				if( vBkgndListBlock == NULL )			throw runtime_error( "XStackFile::GetBkgndByID - block list missing from file." );	}		vFoundBkgnd = vBkgndListBlock->GetBlockEntry( theID );	if( vFoundBkgnd == NULL && create )		vFoundBkgnd = mBlockFile.AddSubBlock( vBkgndListBlock, theID, &vDummy, sizeof(vDummy) );		return vFoundBkgnd;}/* --------------------------------------------------------------------------------	GetPartByID:		Get a reference to a part which can then be further manipulated.		TAKES:		cardOrBg	-	the card or background the part belongs to.						Note that this does not resolve unshared properties.		theID		-	ID of the part to get.		create		-	Set this to TRUE if you want a block created for you when						there is none. Defaults to FALSE.		GIVES:		-		REVISIONS:		2000-10-01	UK		Added 'create' parameter.		2000-07-08	UK		Created.   ----------------------------------------------------------------------------- */XSFPartRef	XStackFile::GetPartByID( XSFLayerRef cardOrBg, XBlockID theID, bool create ){	XBlockEntry*		vPartListBlock;	XSFPartRef			vFoundPart;		vPartListBlock = cardOrBg->GetBlockEntry( LAYER_PART_LIST_BLOCK_ID );	if( vPartListBlock == NULL )	{		if( create )			vPartListBlock = mBlockFile.AddSubBlock( cardOrBg, LAYER_PART_LIST_BLOCK_ID, &vDummy, sizeof(vDummy) );				if( vPartListBlock == NULL )			throw runtime_error( "XStackFile::GetPartByID - block list missing from file." );	}		vFoundPart = vPartListBlock->GetBlockEntry( theID );	if( vFoundPart == NULL && create )		vFoundPart = mBlockFile.AddSubBlock( vPartListBlock, theID, &vDummy, sizeof(vDummy) );		return vFoundPart;}/* --------------------------------------------------------------------------------	GetProperty:		Get a fixed or flexible size property from any entity.		TAKES:		entity		-	Any entity from which you want to get a property.		propID		-	ID of the property to get.		outSize		-	A variable to return the data size in. May be NULL.		create		-	Set this to TRUE if you want a block created for you when						there is none. Defaults to FALSE.		GIVES:		outSize		-	Size of the data to which the pointer was returned.		REVISIONS:		2000-10-01	UK		Added 'create' parameter.		2000-07-08	UK		Created.   ----------------------------------------------------------------------------- */void*	XStackFile::GetProperty( XSFEntityRef entity, long propID, long *outSize, bool create ){	XBlockEntry		*vPropList,					*vFixedPropTable,					*vPropertyBlock;	XBlockOffset	vDummy = 0;		vPropList = entity->GetBlockEntry( PROPERTY_LIST_BLOCK_ID );	if( vPropList == NULL )	{		if( create )			vPropList = mBlockFile.AddSubBlock( entity, PROPERTY_LIST_BLOCK_ID, &vDummy, sizeof(vDummy) );				if( vPropList == NULL )			throw runtime_error( "XStackFile::GetProperty - property list missing from file." );	}		if( propID >= DYN_PROP_START_ID )	// Dynamic size property:	{		vPropertyBlock = vPropList->GetBlockEntry( propID );	// Just get its data from its block.		if( vPropertyBlock == NULL )		{			if( create )				vPropertyBlock = mBlockFile.AddSubBlock( vPropList, propID, &vDummy, sizeof(vDummy) );						if( vPropertyBlock == NULL )				throw runtime_error( "XStackFile::GetProperty - dynamic size property block missing from file." );		}		if( outSize != NULL )			*outSize = vPropertyBlock->GetLength();		return mBlockFile.GetBlockData( vPropertyBlock );	}	else	// Fixed size property:	{		size_t		vOffset,					vDataOffset;				// Get offset table:		vFixedPropTable = vPropList->GetBlockEntry( FIXED_SIZE_PROP_TABLE_BLOCK_ID );		if( vFixedPropTable == NULL )		{			if( create )				vFixedPropTable = mBlockFile.AddSubBlock( vPropList, FIXED_SIZE_PROP_TABLE_BLOCK_ID, &vDummy, sizeof(vDummy) );						if( vFixedPropTable == NULL )				throw runtime_error( "XStackFile::GetProperty - fixed size property offset table missing from file." );		}				// FIX ME! Stuff below fails if you create a block for a property!		// Read offset from table:		vOffset = propID * sizeof(long);		mBlockFile.StreamFromBlockEntry( vFixedPropTable, &vDataOffset, sizeof(long), vOffset );				// Get property data block:		vPropertyBlock = vPropList->GetBlockEntry( FIXED_SIZE_PROPERTIES_BLOCK_ID );		if( vPropertyBlock == NULL )		{			if( create )				vPropertyBlock = mBlockFile.AddSubBlock( vPropList, FIXED_SIZE_PROPERTIES_BLOCK_ID, &vDummy, sizeof(vDummy) );						if( vPropertyBlock == NULL )				throw runtime_error( "XStackFile::GetProperty - fixed size property data block missing from file." );		}		// Add to pointer and return it:		char*		vPtr;				vPtr = (char*) mBlockFile.GetBlockData( vPropertyBlock );		vPtr += vDataOffset;				return vPtr;	}}