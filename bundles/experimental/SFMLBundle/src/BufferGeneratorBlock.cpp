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
#include "BufferGeneratorBlock.h"
#include "RessourceManagerBlock.h"

#include "OpenGl.h"
#include "RessourceManager.h"
#include "Datatypes.h"
#include "helpers\_2RealVectorInitializer.h"

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

SkeletonsToBufferBlock::SkeletonsToBufferBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr ), mVertexBufferObj( nullptr ), mBoneBufferObj( nullptr )
{
}

SkeletonsToBufferBlock::~SkeletonsToBufferBlock()
{
}

void SkeletonsToBufferBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mBufferDataIn = mBlockHandle.getInletHandle( "BufferData" );
		mVertexBufferOut = mBlockHandle.getOutletHandle( "VertexBuffer" );
		mBoneBufferOut = mBlockHandle.getOutletHandle( "BoneBuffer" );

		if ( mContext == nullptr )
		{
			mContext = new Context( mManager.getRenderSettings(), mManager.getManager() );
		}

		mContext->setActive( true );
		updateBuffers( mBufferDataIn.getReadableRef< vector< Skeleton > >() );
		mContext->finish();
		mContext->setActive( false );
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

SkeletonsToBufferBlock::BoneIndices SkeletonsToBufferBlock::findBoneIndices( vector< RigidBody > const& rigidBodies, string const& l1, string const& l2 )
{
	BoneIndices indices;

	indices.first = -1;
	indices.second = -1;

	for ( unsigned int i=0; i<rigidBodies.size(); ++i )
	{
		if ( rigidBodies[ i ].getLabel() == l1 ) indices.first = i;
		else if ( rigidBodies[ i ].getLabel() == l2 ) indices.second = i;
	}

	return indices;
}

void SkeletonsToBufferBlock::updateBuffers( vector< Skeleton > const& skeletons )
{
	vector< float > points;
	vector< unsigned int > bones;

	unsigned int numSkeletonPoints = 0;
	for ( vector< Skeleton >::const_iterator sIt = skeletons.begin(); sIt != skeletons.end(); ++sIt )
	{
		Skeleton &s = const_cast< Skeleton & >( *sIt );
		vector< RigidBody > const& rb = s.getRigidBodies();
		for ( vector< RigidBody >::const_iterator bIt = rb.begin(); bIt != rb.end(); ++bIt )
		{
			RigidBody const& b = *bIt;
			if ( b.hasPosition() )
			{
				++numSkeletonPoints;
				points.push_back( b.getPosition().x() );
				points.push_back( b.getPosition().y() );
				points.push_back( b.getPosition().z() );
			}
		}

		vector< pair< string, string > > labels;
		labels.push_back( make_pair( "left shoulder", "right shoulder" ) );
		labels.push_back( make_pair( "left hip", "right hip" ) );
		labels.push_back( make_pair( "right shoulder", "torso" ) );
		labels.push_back( make_pair( "left shoulder", "torso" ) );
		labels.push_back( make_pair( "right hip", "torso" ) );
		labels.push_back( make_pair( "left hip", "torso" ) );
		labels.push_back( make_pair( "left shoulder", "left elbow" ) );
		labels.push_back( make_pair( "left elbow", "left hand" ) );
		labels.push_back( make_pair( "right shoulder", "right elbow" ) );
		labels.push_back( make_pair( "right elbow", "right hand" ) );
		labels.push_back( make_pair( "head", "neck" ) );
		labels.push_back( make_pair( "neck", "torso" ) );
		labels.push_back( make_pair( "right hip", "right knee" ) );
		labels.push_back( make_pair( "right knee", "right foot" ) );
		labels.push_back( make_pair( "left hip", "left knee" ) );
		labels.push_back( make_pair( "left knee", "left foot" ) );

		for ( vector< pair< string, string > >::const_iterator it = labels.begin(); it != labels.end(); ++it )
		{
			pair< int, int > indices;

			indices.first = -1;
			indices.second = -1;

			for ( unsigned int i=0; i<rb.size(); ++i )
			{
				if ( rb[ i ].getLabel() == it->first ) indices.first = i;
				else if ( rb[ i ].getLabel() == it->second ) indices.second = i;
			}

			if ( ( indices.first >= 0 ) && ( indices.second >= 0 ) )
			{
				bones.push_back( static_cast< unsigned int >( indices.first ) );
				bones.push_back( static_cast< unsigned int >( indices.second ) );
			}
		}
	}

	size_t pointsBufferSize = numSkeletonPoints * 3 * sizeof( float );

	if ( mVertexBufferObj == nullptr || mVertexBufferObj->mSizeInBytes != pointsBufferSize )
	{
		mVertexBufferObj = mContext->createBufferObj();
		mVertexBuffer.reset( mVertexBufferObj );
	}

	const size_t boneBufferSize = bones.size() * sizeof( unsigned int );

	if ( mBoneBufferObj == nullptr || mBoneBufferObj->mSizeInBytes != boneBufferSize )
	{
		mBoneBufferObj = mContext->createBufferObj();
		mBoneBuffer.reset( mBoneBufferObj );
	}

	mVertexBufferObj->mLock.writeLock();
	mContext->updateBuffer( mVertexBufferObj, points, GL_DYNAMIC_DRAW );
	mVertexBufferObj->mLock.unlock();

	mBoneBufferObj->mLock.writeLock();
	mContext->updateBuffer( mBoneBufferObj, bones, GL_DYNAMIC_DRAW );
	mBoneBufferObj->mLock.unlock();
}

void SkeletonsToBufferBlock::update()
{
	try
	{
		if ( mBufferDataIn.hasUpdated() )
		{
			mContext->setActive( true );
			updateBuffers( mBufferDataIn.getReadableRef< vector< Skeleton > >() );
			mContext->finish();
			mContext->setActive( false );
		}

		mVertexBufferOut.getWriteableRef< Buffer >() = mVertexBuffer;
		mBoneBufferOut.getWriteableRef< Buffer >() = mBoneBuffer;
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

void SkeletonsToBufferBlock::shutdown()
{
	try
	{
		mVertexBufferObj = nullptr;
		mBoneBufferObj = nullptr;
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