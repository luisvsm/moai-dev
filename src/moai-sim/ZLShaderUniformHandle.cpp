// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAttribute.h>
#include <moai-sim/ZLShaderUniformHandle.h>

//================================================================//
// ZLShaderUniformHandle
//================================================================//

//----------------------------------------------------------------//
void ZLShaderUniformHandle::AddValue ( const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				*( float* )this->mBuffer += value;
			}
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {

			int value = ( int )attr.GetValue ( 0 );

			if ( value != 0 ) {
				*( s32* )this->mBuffer += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::Default ( u32 count ) {
	
	if ( this->mWidth == 9 ) {
	
		ZLShaderUniformHandle cursor = *this;
	
		for ( u32 i = 0; i < count; ++i ) {
			cursor.SetValue ( ZLMatrix3x3::IDENT );
			cursor.Next ();
		}
	}
	else if ( this->mWidth == 16 ) {
	
		ZLShaderUniformHandle cursor = *this;
	
		for ( u32 i = 0; i < count; ++i ) {
			cursor.SetValue ( ZLMatrix4x4::IDENT );
			cursor.Next ();
		}
	}
	else {
	
		memset ( this->mBuffer, 0, this->mWidth * count * ELEMENT_SIZE );
	}
}

//----------------------------------------------------------------//
bool ZLShaderUniformHandle::IsValid () {

	return ( this->mBuffer != NULL );
}

//----------------------------------------------------------------//
ZLShaderUniformHandle::ZLShaderUniformHandle () {
}

//----------------------------------------------------------------//
ZLShaderUniformHandle::~ZLShaderUniformHandle () {
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::Next () {

	this->mBuffer = ( void* )(( size_t )this->mBuffer + ( this->mWidth * ELEMENT_SIZE ));
}

//----------------------------------------------------------------//
ZLShaderUniformHandle ZLShaderUniformHandle::Offset ( ZLIndex index ) {

	ZLShaderUniformHandle offset = *this;
	offset.mBuffer = ( void* )(( size_t )this->mBuffer + ( this->mWidth * ELEMENT_SIZE * ( ZLSize )index ));
	return offset;
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetRawValue ( const void* srcBuffer ) {
	
	memcpy ( this->mBuffer, srcBuffer, this->mWidth * ELEMENT_SIZE );
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const s32* srcBuffer ) {

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		float fBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			fBuffer [ i ] = ( float )srcBuffer [ i ];
		}
		return this->SetRawValue ( fBuffer );
	}
	return this->SetRawValue ( srcBuffer );
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const float* srcBuffer ) {

	if ( this->mType == UNIFORM_TYPE_INT ) {
	
		s32 iBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			iBuffer [ i ] = ( s32 )srcBuffer [ i ];
		}
		return this->SetRawValue ( iBuffer );
	}
	return this->SetRawValue ( srcBuffer );
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( float value ) {

	if ( this->mType == ZLShaderUniformHandle::UNIFORM_TYPE_INT ) {
		this->SetValue (( s32 )value );
		return;
	}
	
	*( float* )this->mBuffer = value;
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( s32 value ) {

	if ( this->mType == ZLShaderUniformHandle::UNIFORM_TYPE_FLOAT ) {
		this->SetValue (( float )value );
		return;
	}

	*( s32* )this->mBuffer = value;
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_COLOR_VEC_4: {
			ZLColorVec color = attr.GetValue ( ZLColorVec::WHITE );
			this->SetValue ( color );
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {
			this->SetValue (( float )attr.GetValue ( 0.0f ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			return this->SetValue (( int )attr.GetValue ( 0 ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_AFFINE_3D: {
			this->SetValue ( attr.GetValue ( ZLAffine3D::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_3X3: {
			this->SetValue ( attr.GetValue ( ZLMatrix3x3::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_4X4: {
			this->SetValue ( attr.GetValue ( ZLMatrix4x4::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_VEC_3: {
			ZLVec3D vec3 = attr.GetValue ( ZLVec3D::ORIGIN );
			ZLVec4D vec4 ( vec3.mX, vec3.mY, vec3.mZ, 0.0f );
			this->SetValue ( vec4 );
			break;
		}
	}
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLColorVec& value ) {

	if ( this->mWidth == 1 ) {
		this->SetValue ( value.mA );
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	this->SetValue ( m );
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLIntVec4D& value ) {

	if ( this->mWidth == 1 ) {
		this->SetValue ( value.mX );
		return;
	}

	s32 m [ 4 ];
	
	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( m );
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLAffine2D& value ) {

	if ( this->mWidth == 9 ) {
	
		this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		this->SetValue ( ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLAffine3D& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLMatrix3x3& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( value.m );
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLMatrix4x4& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( value.m );
	}
}

//----------------------------------------------------------------//
void ZLShaderUniformHandle::SetValue ( const ZLVec4D& value ) {

	if ( this->mWidth == 1 ) {
		this->SetValue ( value.mX );
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( m );
}