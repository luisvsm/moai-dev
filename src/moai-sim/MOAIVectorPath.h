// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORPATH_H
#define	MOAIVECTORPATH_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorPath
//================================================================//
class MOAIVectorPath :
	public MOAIVectorShape {
protected:

	ZLLeanArray < ZLVec2D > mVertices;

public:
	
	//----------------------------------------------------------------//
	void			AddFillContours			( TESStesselator* tess );
	void			AddStrokeContours		( TESStesselator* tess );
					MOAIVectorPath			();
					~MOAIVectorPath			();
	bool			SetVertices				( const ZLVec2D* vertices, u32 total );
};

#endif
