#include "BufferGeneratorBlock.h"
#include "RessourceManagerBlock.h"

#include "OpenGl.h"
#include "RessourceManager.h"
#include "Datatypes.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

VectorToBufferBlock::VectorToBufferBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr )
{
}

VectorToBufferBlock::~VectorToBufferBlock()
{
}

void VectorToBufferBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mBufferDataIn = mBlockHandle.getInletHandle( "BufferData" );
		mBufferOut = mBlockHandle.getOutletHandle( "Buffer" );

		// context = created on very first setup, never changes
		if ( mContext == nullptr )
		{
			RenderSettings settings;
			settings.title = "";
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;
			settings.width = 640;
			settings.height = 480;

			mContext = new Context( settings, mManager.getManager() );

			mBufferObj = mContext->createBufferObj();
			mBuffer.reset( mBufferObj );
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void VectorToBufferBlock::update()
{
	try
	{
		Buffer& b = mBufferOut.getWriteableRef< Buffer >();

		if ( mBufferDataIn.hasUpdated() )
		{
			vector< float > const& bufferData = mBufferDataIn.getReadableRef< vector< float > >();
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
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void VectorToBufferBlock::shutdown()
{
	try
	{
		mBufferObj = nullptr;
		delete mContext;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}