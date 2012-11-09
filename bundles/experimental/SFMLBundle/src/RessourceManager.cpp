#include "RessourceManager.h"
#include "OpenGl.h"
#include "Datatypes.h"

using namespace std;

namespace _2Real
{
	namespace gl
	{
		RessourceManager::RessourceManager( RenderSettings const& s ) :
			mSfContext( sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ), s.width, s.height ),
			mGlewContext( new GLEWContext() ),
			mContextSettings( s )
		{
			GLenum success = glewContextInit( mGlewContext.get() );
			if ( success != GLEW_OK )
			{
				throw std::exception( "could not create a GLEW context" );
			}
			else
			{
				success = glewInit();
				if ( success != GLEW_OK )
				{
					throw std::exception( "could not initialize GLEW" );
				}
			}
		}

		void RessourceManager::update()
		{
			Poco::ScopedLock< Poco::FastMutex > lock( mMutex );

			mSfContext.setActive( true );

			for ( Handles::iterator it = mTexturesToDelete.begin(); it != mTexturesToDelete.end(); ++it )
			{
				std::cout << "ressource manager: deleting texture " << *it << std::endl;
				GLuint tmp = *it;
				glDeleteTextures( 1, &tmp );
			}
			mTexturesToDelete.clear();

			for ( Handles::iterator it = mBuffersToDelete.begin(); it != mBuffersToDelete.end(); ++it )
			{
				std::cout << "ressource manager: deleting buffer " << *it << std::endl;
				GLuint tmp = *it;
				glDeleteBuffers( 1, &tmp );
			}
			mBuffersToDelete.clear();

			for ( Handles::iterator it = mProgramsToDelete.begin(); it != mProgramsToDelete.end(); ++it )
			{
				std::cout << "ressource manager: deleting program " << *it << std::endl;
				GLuint tmp = *it;
				glDeleteProgram( tmp );
			}
			mProgramsToDelete.clear();

			for ( Handles::iterator it = mShadersToDelete.begin(); it != mShadersToDelete.end(); ++it )
			{
				std::cout << "ressource manager: deleting shader " << *it << std::endl;
				GLuint tmp = *it;
				glDeleteShader( tmp );
			}
			mShadersToDelete.clear();

			mSfContext.setActive( false );
		}

		RessourceManager::~RessourceManager()
		{
			std::cout << "ressource manager: dtor" << std::endl;
		}

		void RessourceManager::destroyTexture( const GLuint handle ) const
		{
			std::cout << "ressource manager: destroy texture" << std::endl;
			Poco::ScopedLock< Poco::FastMutex > lock( mMutex );
			if ( !mIsEnabled ) return;
			std::cout << "ressource manager: stored texture handle" << std::endl;
			mTexturesToDelete.push_back( handle );
		}

		void RessourceManager::destroyBuffer( const GLuint handle ) const
		{
			std::cout << "ressource manager: destroy buffer" << std::endl;
			Poco::ScopedLock< Poco::FastMutex > lock( mMutex );
			if ( !mIsEnabled ) return;
			std::cout << "ressource manager: stored buffer handle" << std::endl;
			mBuffersToDelete.push_back( handle );
		}

		void RessourceManager::destroyProgram( const GLuint handle ) const
		{
			std::cout << "ressource manager: destroy program" << std::endl;
			Poco::ScopedLock< Poco::FastMutex > lock( mMutex );
			if ( !mIsEnabled ) return;
			std::cout << "ressource manager: stored program handle" << std::endl;
			mProgramsToDelete.push_back( handle );
		}

		void RessourceManager::destroyShader( const GLuint handle ) const
		{
			std::cout << "ressource manager: destroy shader" << std::endl;
			Poco::ScopedLock< Poco::FastMutex > lock( mMutex );
			if ( !mIsEnabled ) return;
			std::cout << "ressource manager: stored shader handle" << std::endl;
			mShadersToDelete.push_back( handle );
		}
	}
}