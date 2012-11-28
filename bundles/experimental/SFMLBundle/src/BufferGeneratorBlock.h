/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#pragma once

#include "_2RealBlock.h"

#include "gl.h"
#include <array>

class RessourceManagerBlock;

template< typename T >
class VectorToBufferBlock : public _2Real::bundle::Block
{

public:

	VectorToBufferBlock( _2Real::bundle::ContextBlock &context );
	~VectorToBufferBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mBufferDataIn;
	_2Real::bundle::OutletHandle		mBufferOut;

	_2Real::gl::BufferObj				*mBufferObj;	// modifieable
	_2Real::gl::Buffer					mBuffer;		// constant

};

template< typename T >
VectorToBufferBlock< T >::VectorToBufferBlock( _2Real::bundle::ContextBlock &context ) :
	_2Real::bundle::Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr ), mBufferObj( nullptr )
{
}

template< typename T >
VectorToBufferBlock< T >::~VectorToBufferBlock()
{
}

template< typename T >
void VectorToBufferBlock< T >::setup( _2Real::bundle::BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mBufferDataIn = mBlockHandle.getInletHandle( "BufferData" );
		mBufferOut = mBlockHandle.getOutletHandle( "Buffer" );

		if ( mContext == nullptr )
		{
			mContext = new Context( mManager.getRenderSettings(), mManager.getManager() );
			mBufferObj = mContext->createBufferObj();
			mBuffer.reset( mBufferObj );
		}
	}
	catch( Exception & e )
	{
		std::cout << e.message() << " " << e.what() << std::endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		std::cout << e.what() << std::endl;
		_2Real::Exception exc( e.what() );
		throw exc;
	}
}

template< typename T >
void VectorToBufferBlock< T >::update()
{
	try
	{
		Buffer& b = mBufferOut.getWriteableRef< Buffer >();

		if ( mBufferDataIn.hasUpdated() )
		{
			vector< T > const& bufferData = mBufferDataIn.getReadableRef< vector< T > >();
			mContext->setActive( true );
			mContext->updateBuffer( mBufferObj, bufferData, GL_DYNAMIC_DRAW );
			mContext->finish();
			mContext->setActive( false );
		}

		if ( mBufferObj->mElementCount > 0 )
		{
			b = mBuffer;
		}
		else
		{
			mBufferOut.discard();
		}
	}
	catch( Exception & e )
	{
		std::cout << e.message() << " " << e.what() << std::endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		std::cout << e.what() << std::endl;
		_2Real::Exception exc( e.what() );
		throw exc;
	}
}

template< typename T >
void VectorToBufferBlock< T >::shutdown()
{
	try
	{
		mBufferObj = nullptr;
		delete mContext;
	}
	catch( Exception & e )
	{
		std::cout << e.message() << " " << e.what() << std::endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		std::cout << e.what() << std::endl;
		_2Real::Exception exc( e.what() );
		throw exc;
	}
}

class ImageToBufferBlock : public _2Real::bundle::Block
{

public:

	ImageToBufferBlock( _2Real::bundle::ContextBlock &context );
	~ImageToBufferBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mBufferDataIn;
	_2Real::bundle::OutletHandle		mBufferOut;

	_2Real::gl::BufferObj				*mBufferObj;	// modifieable
	_2Real::gl::Buffer					mBuffer;		// constant

};

class SkeletonsToBufferBlock : public _2Real::bundle::Block
{

public:

	SkeletonsToBufferBlock( _2Real::bundle::ContextBlock &context );
	~SkeletonsToBufferBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	typedef std::pair< int, int > BoneIndices;

	static BoneIndices findBoneIndices( std::vector< _2Real::RigidBody > const& rigidBodies, std::string const& l1, std::string const& l2 );

	void updateBuffers( std::vector< _2Real::Skeleton > const& skeletons );

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mBufferDataIn;
	_2Real::bundle::OutletHandle		mVertexBufferOut;
	_2Real::bundle::OutletHandle		mBoneBufferOut;

	_2Real::gl::BufferObj				*mVertexBufferObj;	// modifieable
	_2Real::gl::Buffer					mVertexBuffer;		// constant

	_2Real::gl::BufferObj				*mBoneBufferObj;	// modifieable
	_2Real::gl::Buffer					mBoneBuffer;		// constant

};