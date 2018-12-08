// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPathTerrainDeck.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getMask
	@text	Returns mask for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@out	number mask
*/
int MOAIPathTerrainDeck::_getMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	ZLIndex idx = state.GetValueAsIndex ( 2 );
	
	if ( idx < self->mMasks.Size ()) {
		lua_pushnumber ( state, self->mMasks [ idx ]);
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getTerrainVec
	@text	Returns terrain vector for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@out	...
*/
int MOAIPathTerrainDeck::_getTerrainVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	ZLIndex idx 	= state.GetValueAsIndex ( 2 );
	float* vector 	= self->GetVector ( idx + ( ZLSize )1 );

	ZLSize size = self->mVectorSize;
	lua_checkstack ( L, size );
	
	for ( ZLIndex i = ZLIndex::ZERO; i < size; ++i ) {
		lua_pushnumber ( state, vector [ i ]);
	}
	return size;
}

//----------------------------------------------------------------//
/**	@lua	setMask
	@text	Returns mask for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@in		number mask
	@out	nil
*/
int MOAIPathTerrainDeck::_setMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UNN" )
	
	ZLIndex idx 	= state.GetValueAsIndex ( 2 );
	u32 mask		= state.GetValue < int >( 3, 0 );
	
	if ( idx < self->mMasks.Size ()) {
		self->mMasks [ idx ] = mask;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTerrainVec
	@text	Sets terrain vector for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@in		float... values
	@out	nil
*/
int MOAIPathTerrainDeck::_setTerrainVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	ZLIndex idx 	= state.GetValueAsIndex ( 2 );
	ZLSize total	= lua_gettop ( state ) - 2;
	
	if ( total > self->mVectorSize ) {
		total = self->mVectorSize;
	}
	
	float* vector = self->GetVector ( idx + ( ZLSize )1 );
	
	for ( ZLIndex i = ZLIndex::ZERO; i < total; ++i ) {
		vector [ i ] = state.GetValue < float >( 3 + i, 0.0 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Allocates terrain vectors.

	@in		MOAIPathTerrainDeck self
	@in		number deckSize
	@in		number terrainVecSize
	@out	nil
*/
int MOAIPathTerrainDeck::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	self->mDeckSize		= state.GetValue < MOAILuaState::SizeType >( 2, 0 );
	self->mVectorSize	= state.GetValue < MOAILuaState::SizeType >( 3, 0 );
	
	self->mMasks.Init ( self->mDeckSize );
	self->mMasks.Fill ( 0xffffffff );
	
	self->mVectors.Init ( self->mDeckSize * self->mVectorSize );
	self->mVectors.Fill ( 0.0f );

	return 0;
}

//================================================================//
// MOAIPathTerrainDeck
//================================================================//

//----------------------------------------------------------------//
u32 MOAIPathTerrainDeck::GetMask ( u32 idx ) {

	return this->mMasks [ ZLIndex ( idx - 1, ZLIndex::LIMIT )];
}

//----------------------------------------------------------------//
float* MOAIPathTerrainDeck::GetVector ( u32 idx ) {

	return &this->mVectors [ ZLIndex (( idx - 1 ) * this->mVectorSize, ZLIndex::LIMIT )];
}

//----------------------------------------------------------------//
MOAIPathTerrainDeck::MOAIPathTerrainDeck () :
	mDeckSize ( 0 ),
	mVectorSize ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPathTerrainDeck::~MOAIPathTerrainDeck () {
}

//----------------------------------------------------------------//
void MOAIPathTerrainDeck::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathTerrainDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getMask",				_getMask },
		{ "getTerrainVec",			_getTerrainVec },
		{ "setMask",				_setMask },
		{ "setTerrainVec",			_setTerrainVec },
		{ "reserve",				_reserve },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
