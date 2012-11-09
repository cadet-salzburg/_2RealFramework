#include "OpenGl.h"
#include "RessourceManager.h"
#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

RessourceManagerBlock::RessourceManagerBlock() :
	ContextBlock(), mManager( nullptr )
{
}

RessourceManagerBlock::~RessourceManagerBlock()
{
}

RessourceManager const& RessourceManagerBlock::getManager() const
{
	return *mManager;
}

RenderSettings const& RessourceManagerBlock::getRenderSettings() const
{
	return mManager->getContextSettings();
}

void RessourceManagerBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;

		if ( mManager == nullptr )
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

			mManager = new RessourceManager( settings );
			mManager->setEnabled( true );
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
		mManager->update();
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
		mManager->setEnabled( false );
		mManager->update();

		delete mManager;

		std::cout << "bundle context shutdown - deleted ressource manager" << std::endl;
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