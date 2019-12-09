// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXMGRONETRIVK_H
#define MOAIGFXMGRONETRIVK_H

#include <moai-gfx-vk/MOAIGfxBufferVK.h>

class MOAICommandBufferVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetVK;
class MOAIGfxBufferSnapshotVK;
class MOAIIndexBufferVK;
class MOAIPipelineLayoutVK;
class MOAIShaderProgramVK;
class MOAIShaderVK;
class MOAITexture2DVK;
class MOAIVertexBufferVK;

//================================================================//
// MOAIGfxMgrOneTriVK
//================================================================//
class MOAIGfxMgrOneTriVK :
	public virtual MOAIDrawable {
private:

	struct Vertex {
		ZLVec4D32	mPosition;
		ZLVec2D32 	mUV;
		u32			mRGBA32;
		
		Vertex ( float x, float y, float z, float r, float g, float b, float u, float v ) {
			this->mPosition.Init ( x, y, z, 1.0 );
			this->mUV.Init ( u, v );
			this->mRGBA32 = ZLColor::PackRGBA ( r, g, b, 1.0 );
		}
	};

	ZLStrongPtr < MOAIVertexBufferVK >		mVertices;
	ZLStrongPtr < MOAIIndexBufferVK >		mIndices;
	ZLStrongPtr < MOAITexture2DVK >			mTexture;
	
	uint32_t				mTotalIndices;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

	//----------------------------------------------------------------//
	void			UpdateMatrices				( u32 width, u32 height );

	//----------------------------------------------------------------//
	void			MOAIDrawable_Draw			( int subPrimID );
	void			MOAIDrawable_DrawDebug		( int subPrimID );

public:

	DECL_LUA_FACTORY ( MOAIGfxMgrOneTriVK )

	//----------------------------------------------------------------//
					MOAIGfxMgrOneTriVK			();
					~MOAIGfxMgrOneTriVK			();
};

#endif