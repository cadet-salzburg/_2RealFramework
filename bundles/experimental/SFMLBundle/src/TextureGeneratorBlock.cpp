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
#include "TextureGeneratorBlock.h"
#include "RessourceManagerBlock.h"

#include "OpenGl.h"
#include "RessourceManager.h"
#include "Datatypes.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

RandomTexture2DBlock::RandomTexture2DBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr )
{
}

RandomTexture2DBlock::~RandomTexture2DBlock()
{
}

void RandomTexture2DBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mTextureWidthIn = mBlockHandle.getInletHandle( "TextureWidth" );
		mTextureHeightIn = mBlockHandle.getInletHandle( "TextureHeight" );
		mTextureOut = mBlockHandle.getOutletHandle( "Texture" );

		if ( mContext == nullptr )
		{
			mContext = new Context( mManager.getRenderSettings(), mManager.getManager() );
			mTextureObj = mContext->createTextureObj();
			mTexture.reset( mTextureObj );
			srand( static_cast< unsigned int >( time( NULL ) ) );
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

void RandomTexture2DBlock::update()
{
	try
	{
		unsigned int w = mTextureWidthIn.getReadableRef< unsigned int >();
		unsigned int h = mTextureHeightIn.getReadableRef< unsigned int >();
		Texture &t = mTextureOut.getWriteableRef< Texture >();

		ImageT< unsigned char > textureData( w, h, ImageChannelOrder::RGBA );
		ImageT< unsigned char >::iterator it = textureData.iter();
		while( it.nextLine() )
		{
			while ( it.nextPixel() )
			{
				it.r() = rand()%255 + 1;
				it.g() = rand()%255 + 1;
				it.b() = rand()%255 + 1;
				it.a() = rand()%255 + 1;
			}
		}

		mContext->setActive( true );
		mTextureObj->mLock.writeLock();
		mContext->updateTexture( mTextureObj, textureData, GL_TEXTURE_2D );
		mContext->finish();
		mTextureObj->mLock.unlock();
		mContext->setActive( false );

		if ( !mTextureObj->isEmpty() )
		{
			t = mTexture;
		}
		else
		{
			mTextureOut.discard();
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

void RandomTexture2DBlock::shutdown()
{
	try
	{
		mTextureObj = nullptr;
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

ImageToTexture2DBlock::ImageToTexture2DBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr )
{
}

ImageToTexture2DBlock::~ImageToTexture2DBlock()
{
}

void ImageToTexture2DBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mTextureDataIn = mBlockHandle.getInletHandle( "TextureData" );
		mTextureOut = mBlockHandle.getOutletHandle( "Texture" );

		if ( mContext == nullptr )
		{
			mContext = new Context( mManager.getRenderSettings(), mManager.getManager() );
			mTextureObj = mContext->createTextureObj();
			mTexture.reset( mTextureObj );
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

void ImageToTexture2DBlock::update()
{
	try
	{
		Image const& textureData = mTextureDataIn.getReadableRef< Image >();
		Texture &t = mTextureOut.getWriteableRef< Texture >();

		mContext->setActive( true );
		mTextureObj->mLock.writeLock();
		mContext->updateTexture( mTextureObj, textureData, GL_TEXTURE_2D );
		mContext->finish();
		mTextureObj->mLock.unlock();
		mContext->setActive( false );

		if ( !mTextureObj->isEmpty() )
		{
			t = mTexture;
		}
		else
		{
			mTextureOut.discard();
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

void ImageToTexture2DBlock::shutdown()
{
	try
	{
		mTextureObj = nullptr;
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