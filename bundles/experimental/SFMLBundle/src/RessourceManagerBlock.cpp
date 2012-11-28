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

		//std::cout << "bundle context shutdown - deleted ressource manager" << std::endl;
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