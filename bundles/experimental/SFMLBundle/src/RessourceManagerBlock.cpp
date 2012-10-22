#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

RessourceManagerBlock::RessourceManagerBlock() : ContextBlock(), m_RessourceManager( nullptr ) {}

RessourceManagerBlock::~RessourceManagerBlock() {}

void RessourceManagerBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		if ( m_RessourceManager == nullptr )
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

			m_RessourceManager = new GlRessourceManager( settings );

			m_RessourceManager->setActive( false );
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

void RessourceManagerBlock::update()
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

		m_RessourceManager->setActive( true );
		m_RessourceManager->cleanUp();

		for ( std::deque< Tex * >::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it )
		{
			( *it )->texture = m_RessourceManager->createTexture( ( *it )->target );
			( *it )->creation.set();
		}

		m_Textures.clear();

		for ( std::deque< Buf * >::iterator it = m_Buffers.begin(); it != m_Buffers.end(); ++it )
		{
			( *it )->buffer = m_RessourceManager->createBuffer( ( *it )->usage );
			( *it )->creation.set();
		}

		m_Buffers.clear();

		m_RessourceManager->setActive( false );
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

void RessourceManagerBlock::shutdown()
{
	try
	{
		std::cout << "opengl ressource manager shutdown" << std::endl;

		Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

		m_RessourceManager->setActive( true );

		for ( std::deque< Tex * >::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it )
		{
			std::cout << "ARGH!" << std::endl;
			( *it )->creation.set();
		}

		m_Textures.clear();

		for ( std::deque< Buf * >::iterator it = m_Buffers.begin(); it != m_Buffers.end(); ++it )
		{
			std::cout << "ARGH!" << std::endl;
			( *it )->creation.set();
		}

		m_Buffers.clear();

		m_RessourceManager->setActive( false );

		delete m_RessourceManager;
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

Texture RessourceManagerBlock::createTexture( const GLenum target )
{
	Tex *t = new Tex();
	t->target = target;

	m_Mutex.lock();
	m_Textures.push_back( t );
	m_Mutex.unlock();

	t->creation.wait();

	Texture texture = t->texture;
	delete t;
	return texture;
}

Buffer RessourceManagerBlock::createBuffer( const GLenum usage )
{
	Buf *b = new Buf();
	b->usage = usage;

	m_Mutex.lock();
	m_Buffers.push_back( b );
	m_Mutex.unlock();

	b->creation.wait();

	Buffer buffer = b->buffer;
	delete b;
	return buffer;
}