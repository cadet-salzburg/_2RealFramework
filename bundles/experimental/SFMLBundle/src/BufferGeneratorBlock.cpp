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

ImageToBufferBlock::ImageToBufferBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr )
{
}

ImageToBufferBlock::~ImageToBufferBlock()
{
}

void ImageToBufferBlock::setup( BlockHandle &block )
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

void ImageToBufferBlock::update()
{
	try
	{
		Buffer& b = mBufferOut.getWriteableRef< Buffer >();

		if ( mBufferDataIn.hasUpdated() )
		{
			Image const& bufferData = mBufferDataIn.getReadableRef< Image >();
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

void ImageToBufferBlock::shutdown()
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