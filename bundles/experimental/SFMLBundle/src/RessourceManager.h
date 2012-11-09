#pragma once

#include "gl.h"
#include "Poco/Mutex.h"

namespace _2Real
{
	namespace gl
	{
		/**
		*	special type of context, responsible for destruction of all opengl objects
		**/
		class RessourceManager
		{

		public:

			RessourceManager( RenderSettings const& settings );
			~RessourceManager();

			GlewContext const& getGlewContext() const { return mGlewContext; }

			void setEnabled( const bool enabled ) { mIsEnabled = enabled ; }
			void update();

			void destroyTexture( const GLuint handle ) const;
			void destroyBuffer( const GLuint handle ) const;
			void destroyProgram( const GLuint handle ) const;
			void destroyShader( const GLuint handle ) const;

			RenderSettings const& getContextSettings() const { return mContextSettings; }

		private:

			GlewContext					mGlewContext;
			sf::Context					mSfContext;
			RenderSettings				mContextSettings;

			typedef std::list< GLuint >	Handles;

			mutable Poco::FastMutex		mMutex;
			mutable Handles				mTexturesToDelete;
			mutable Handles				mBuffersToDelete;
			mutable Handles				mProgramsToDelete;
			mutable Handles				mShadersToDelete;
			bool						mIsEnabled;

		};
	}
}