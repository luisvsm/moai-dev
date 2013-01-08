// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDistance.h>
#include <uslscore/USIntersect.h>
#include <uslscore/USTrig.h>
#include <uslscore/USPrism.h>
#include <uslscore/USRhombus.h>
#include <float.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
u32 _bestTime ( float t0, float t1, float& t );
u32 _bestTime ( float t0, float t1, float& t ) {

	if ( t0 >= 0.0f ) {
		t = t0;
	}
	else if ( t1 >= 0.0f ) {
		t = t1;
	}
	else {
		return USSect::SECT_NONE;
	}
	return USSect::SECT_HIT;
}

//----------------------------------------------------------------//
void _clipRayToBoxAxis ( float min, float max, float pos, float dir, float& t0, float& t1 );
void _clipRayToBoxAxis ( float min, float max, float pos, float dir, float& t0, float& t1 ) {
	
	float u0, u1;
	
	u0 = ( min - pos ) / ( dir );
	u1 = ( max - pos ) / ( dir );
	
	if ( u0 > u1 ) {
		float temp = u0;
		u0 = u1;
		u1 = temp;
	}
	
	t0 = MAX ( u0, t0 );
	t1 = MIN ( u1, t1 );
}

//================================================================//
// USSect
//================================================================//

//----------------------------------------------------------------//
// Return:
//	 1:		Box is in front of the plane
//	 0:		Box intersects the plane
//	-1:		Box is behind the plane
s32 USSect::BoxToPlane ( const USBox& b, const USPlane3D& p ) {

	// Get the box spans
	USVec3D spans = b.mMax;
	spans.Sub ( b.mMin );
	spans.Scale ( 0.5f );

	// Get the span dots
	float sdX = spans.mX * p.mNorm.mX;
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = spans.mY * p.mNorm.mY;
	if ( sdY < 0.0f ) sdY = -sdY;

	float sdZ = spans.mZ * p.mNorm.mZ;
	if ( sdZ < 0.0f ) sdZ = -sdZ;

	// Get the radius of the box (as projected onto the plane's normal)
	float r = sdX + sdY + sdZ;

	// Get the box center
	USVec3D c = b.mMin;
	c.Add ( spans );

	// The distance from the center of the box to the plane
	float d = USDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The box is in front of the plane
	if ( d < -r ) return -1; // The box is behind the plane
	return SECT_HIT; // The box intersects the plane
}

//----------------------------------------------------------------//
// Return:
//	 1:		Prism is in front of the plane
//	 0:		Prism intersects the plane
//	-1:		Prism is behind the plane
s32 USSect::PrismToPlane ( const USPrism& prism, const USPlane3D& p ) {

	// Get the span dots
	float sdX = prism.mXAxis.Dot ( p.mNorm );
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = prism.mXAxis.Dot ( p.mNorm );
	if ( sdY < 0.0f ) sdY = -sdY;

	float sdZ = prism.mXAxis.Dot ( p.mNorm );
	if ( sdZ < 0.0f ) sdZ = -sdZ;

	// Get the radius of the prism (as projected onto the plane's normal)
	float r = ( sdX + sdY + sdZ ) * 0.5f;

	// Get the prism center
	USVec3D c;
	prism.GetCenter ( c );

	// The distance from the center of the prism to the plane
	float d = USDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The prism is in front of the plane
	if ( d < -r ) return -1; // The prism is behind the plane
	return SECT_HIT; // The prism intersects the plane
}

//----------------------------------------------------------------//
u32 USSect::RayToBox ( const USVec3D& loc, const USVec3D& vec, const USBox& b, float& t ) {

	float t0;
	float t1;

	if ( USSect::RayToBox ( loc, vec, b, t0, t1 ) == SECT_HIT ) {
		return _bestTime ( t0, t1, t );
	}
	return SECT_NONE;
}

//----------------------------------------------------------------//
u32 USSect::RayToBox ( const USVec3D& loc, const USVec3D& vec, const USBox& b, float& t0, float& t1 ) {

	t0 = -FLT_MAX;
	t1 = FLT_MAX;

	if (( vec.mX == 0.0f ) && ( vec.mY == 0.0f )) return SECT_NONE;
	
	_clipRayToBoxAxis ( b.mMin.mX, b.mMax.mX, loc.mX, vec.mX, t0, t1 );
	_clipRayToBoxAxis ( b.mMin.mY, b.mMax.mY, loc.mY, vec.mY, t0, t1 );
	_clipRayToBoxAxis ( b.mMin.mZ, b.mMax.mZ, loc.mZ, vec.mZ, t0, t1 );

	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::RayToRay ( const USVec2D& locA, const USVec2D& vecA, const USVec2D& locB, const USVec2D& vecB, float &uA, float& uB ) {

	float x0 = locA.mX;
	float x1 = locA.mX + vecA.mX;
	
	float x2 = locB.mX;
	float x3 = locB.mX + vecB.mX;
	
	float y0 = locA.mY;
	float y1 = locA.mY + vecA.mY;
	
	float y2 = locB.mY;
	float y3 = locB.mY + vecB.mY;

	float d = (( y3 - y2 ) * ( x1 - x0 )) - (( x3 - x2 ) * ( y1 - y0 ));
	if ( d != 0.0f ) {
		
		uA = ((( x3 - x2 ) * ( y0 - y2 )) - (( y3 - y2 ) * ( x0 - x2 ))) / d;
		uB = ((( x1 - x0 ) * ( y0 - y2 )) - (( y1 - y0 ) * ( x0 - x2 ))) / d;
		
		return ( 0.0f <= uA ) && ( uA <= 1.0f ) && ( 0.0f <= uB ) && ( uB <= 1.0f ) ? SECT_HIT : SECT_NONE;
	}
	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 USSect::RayToRect ( const USVec2D& loc, const USVec2D& vec, const USRect& r, float &t ) {

	float t0;
	float t1;

	if ( USSect::RayToRect ( loc, vec, r, t0, t1 ) == SECT_HIT ) {
		return _bestTime ( t0, t1, t );
	}
	return SECT_NONE;
}

//----------------------------------------------------------------//
u32 USSect::RayToRect ( const USVec2D& loc, const USVec2D& vec, const USRect& r, float &t0, float& t1 ) {

	t0 = -FLT_MAX;
	t1 = FLT_MAX;

	if (( vec.mX == 0.0f ) && ( vec.mY == 0.0f )) return SECT_NONE;
	
	_clipRayToBoxAxis ( r.mXMin, r.mXMax, loc.mX, vec.mX, t0, t1 );
	_clipRayToBoxAxis ( r.mYMin, r.mYMax, loc.mY, vec.mY, t0, t1 );

	return SECT_HIT;
}

//----------------------------------------------------------------//
// Return:
//	 1:		Rhombus is in front of the plane
//	 0:		Rhombus intersects the plane
//	-1:		Rhombus is behind the plane
s32 USSect::RhombusToPlane ( const USRhombus& rhombus, const USPlane3D& p ) {

	// Get the span dots
	float sdX = rhombus.mXAxis.Dot ( p.mNorm );
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = rhombus.mYAxis.Dot ( p.mNorm );
	if ( sdY < 0.0f ) sdY = -sdY;

	// Get the radius of the rhombus (as projected onto the plane's normal)
	float r = ( sdX + sdY ) * 0.5f;

	// Get the rhombus center
	USVec3D c;
	rhombus.GetCenter ( c );

	// The distance from the center of the rhombus to the plane
	float d = USDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The rhombus is in front of the plane
	if ( d < -r ) return -1; // The rhombus is behind the plane
	return SECT_HIT; // The rhombus intersects the plane
}

//----------------------------------------------------------------//
u32 USSect::VecToCircle ( float& t0, float& t1, const USVec2D& loc, const USVec2D& vec, const USVec2D& circleLoc, float radius ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY );

	b = (( vec.mX * ( loc.mX - circleLoc.mX )) +
		( vec.mY * ( loc.mY - circleLoc.mY ))) * 2.0f;

	c = ((( loc.mX - circleLoc.mX ) * ( loc.mX - circleLoc.mX )) +
		(( loc.mY - circleLoc.mY ) * ( loc.mY - circleLoc.mY )) -
		radius * radius );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		assert ( t0 < t1 );

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 USSect::VecToPlane ( const USVec2D& loc, const USVec2D& vec, const USPlane2D& p, float& t ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::VecToPlane ( const USVec3D& loc, const USVec3D& vec, const USPlane3D& p, float& t ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::VecToPlane ( const USVec3D& loc, const USVec3D& vec, const USPlane3D& p, float& t, USVec3D& result ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	
	result = vec;
	result.Scale ( t );
	result.Add ( loc );
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::VecToSphere ( float& t0, float& t1, const USVec3D& loc, const USVec3D& vec, const USVec3D& sphereLoc, float radius ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY ) + ( vec.mZ * vec.mZ );

	b = (( vec.mX * ( loc.mX - sphereLoc.mX )) +
		( vec.mY * ( loc.mY - sphereLoc.mY )) +
		( vec.mZ * ( loc.mZ - sphereLoc.mZ ))) * 2.0f;

	c = ((( loc.mX - sphereLoc.mX ) * ( loc.mX - sphereLoc.mX )) +
		(( loc.mY - sphereLoc.mY ) * ( loc.mY - sphereLoc.mY )) +
		(( loc.mZ - sphereLoc.mZ ) * ( loc.mZ - sphereLoc.mZ )) -
		radius * radius );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 USSect::VecToUnitCircle ( float& t0, float& t1, const USVec2D& loc, const USVec2D& vec ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY );

	b = (( vec.mX * loc.mX ) +
		( vec.mY * loc.mY )) * 2.0f;

	c = (( loc.mX * loc.mX ) +
		( loc.mY * loc.mY ) -
		1.0f );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		assert ( t0 < t1 );

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 USSect::VecToUnitSphere ( float& t0, float& t1, const USVec3D& loc, const USVec3D& vec ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY ) + ( vec.mZ * vec.mZ );

	b = (( vec.mX * loc.mX ) +
		( vec.mY * loc.mY ) +
		( vec.mZ * loc.mZ )) * 2.0f;

	c = (( loc.mX * loc.mX ) +
		( loc.mY * loc.mY ) +
		( loc.mZ * loc.mZ ) -
		1.0f );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		assert ( t0 < t1 );

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 USSect::XAxisToPlane ( float y, const USPlane2D& p, float& t ) {

	float d;
	d = p.mNorm.mX;
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = (( p.mNorm.mY * y ) + p.mDist ) / -d;
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::YAxisToPlane ( float x, const USPlane2D& p, float& t ) {

	float d;
	d = p.mNorm.mY;
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = (( p.mNorm.mX * x ) + p.mDist ) / -d;
	
	return SECT_HIT;
}
