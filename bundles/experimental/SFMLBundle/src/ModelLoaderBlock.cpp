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
#include "ModelLoaderBlock.h"
#include "RessourceManagerBlock.h"

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

ModelLoaderBlock::ModelLoaderBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr ), mModelLoader()
{}

ModelLoaderBlock::~ModelLoaderBlock() {}

void ModelLoaderBlock::setup( BlockHandle &block )
{
	try
	{
		if ( mContext == nullptr )
		{
			mContext = new Context( mManager.getRenderSettings(), mManager.getManager() );
			mBlockHandle = block;
			mFilePathIn = block.getInletHandle( "FilePath" );
			mIndicesOut = block.getOutletHandle( "Indices" );
			mVertexPositionsOut = block.getOutletHandle( "VertexPositions" );
			mVertexNormalsOut = block.getOutletHandle( "VertexNormals" );
			mNumberOfMeshesOut = block.getOutletHandle( "NumberOfMeshes" );
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

void ModelLoaderBlock::update()
{
	try
	{
		if ( mFilePathIn.hasChanged() )
		{
			cout << "filepath has changed; reloading model" << endl;

			string file = mFilePathIn.getReadableRef< string >();

			mModelLoader.SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE );
			aiScene const* const scene = mModelLoader.ReadFile( file, /*aiProcess_PreTransformVertices | */aiProcess_JoinIdenticalVertices | aiProcess_FindDegenerates | aiProcess_SortByPType | aiProcess_Triangulate | aiProcess_RemoveComponent | aiProcess_ValidateDataStructure );

			if ( scene == nullptr ) cout << "file could not be loaded" << endl;
			else
			{
				Buffer &vertexPositions = mVertexPositionsOut.getWriteableRef< Buffer >();
				Buffer &vertexNormals = mVertexNormalsOut.getWriteableRef< Buffer >();
				Buffer &meshIndices = mIndicesOut.getWriteableRef< Buffer >();

				aiMesh **meshes = scene->mMeshes;
				unsigned int meshCount = scene->mNumMeshes;

				cout << "model has " << meshCount << " meshes" << endl;

				for ( unsigned int i=0; i<meshCount; ++i )
				{
					aiMesh *mesh = meshes[ i ];
					const unsigned int vertexCount = mesh->mNumVertices;
					const unsigned int elementCount = 3 * vertexCount;
					const unsigned int faceCount = mesh->mNumFaces;
					const unsigned int indexCount = 3* faceCount;

					cout << "mesh " << i << " has " << vertexCount << " vertices" << endl;
					cout << "mesh " << i << " has " << mesh->mPrimitiveTypes << " primitive types" << endl;
					if ( mesh->HasFaces() )					cout << "mesh " << i << " has " << faceCount << " faces" << endl;
					if ( mesh->HasNormals() )				cout << "mesh " << i << " has normals" << endl;
					if ( mesh->HasBones() )					cout << "mesh " << i << " has " << mesh->mNumBones << " bones" << endl;
					if ( mesh->HasTangentsAndBitangents() )	cout << "mesh " << i << " has tangents and bitangents" << endl;

					for ( unsigned int j=0; j<AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j )
						if ( mesh->HasTextureCoords( j ) )	cout << "mesh " << i << " has texture coords " << j << endl;
					for ( unsigned int j=0; j<AI_MAX_NUMBER_OF_COLOR_SETS; ++j )
						if ( mesh->HasVertexColors( j ) )	cout << "mesh " << i << " has vertex colors " << j << endl;

					float *verticesAsFloat = reinterpret_cast< float * >( mesh->mVertices );
					std::vector< float > vertices;
					vertices.assign( verticesAsFloat, verticesAsFloat + elementCount );

					float *normalsAsFloat = reinterpret_cast< float * >( mesh->mNormals );
					std::vector< float > normals;
					normals.assign( normalsAsFloat, normalsAsFloat + elementCount );

					BufferObj *v = mContext->createBufferObj();
					mContext->updateBuffer( v, vertices, GL_STREAM_DRAW );
					Buffer vertexBuffer( v );
					vertexPositions = vertexBuffer;

					BufferObj *n = mContext->createBufferObj();
					mContext->updateBuffer( n, normals, GL_STREAM_DRAW );
					Buffer normalBuffer( n );
					vertexNormals = normalBuffer;

					vector< unsigned int > indices( indexCount, 0 );
					vector< unsigned int >::iterator currIndex = indices.begin();
					aiFace *currFace = mesh->mFaces;
					for ( unsigned int j=0; j<mesh->mNumFaces; j++ )
					{
						if ( currFace->mNumIndices != 3 ) assert( NULL );

						*currIndex = currFace->mIndices[ 0 ]; ++currIndex;
						*currIndex = currFace->mIndices[ 1 ]; ++currIndex;
						*currIndex = currFace->mIndices[ 2 ]; ++currIndex;
						++currFace;
					}

					BufferObj *i = mContext->createBufferObj();
					mContext->updateBuffer( i, indices, GL_STREAM_DRAW );
					Buffer indexBuffer( i );
					meshIndices = indexBuffer;

					mNumberOfMeshesOut.getWriteableRef< unsigned int >() = meshCount;
				}
			}
		}

		mContext->setActive( true );
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

void ModelLoaderBlock::shutdown()
{
	try
	{
		mModelLoader.FreeScene();
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