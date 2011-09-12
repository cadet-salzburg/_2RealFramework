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

#pragma once

#include "Poco/ClassLibrary.h"

namespace _2Real
{

	/**
	*
	*/

	class PluginMetadata;
	class PluginContext;

	class IPluginActivator
	{

	public:

		/**
		*	returns plugin metadata
		*/
		virtual void getMetadata(PluginMetadata &_info) throw(...) = 0;

		/**
		*	plugin can use plugin context ptr to export factory functions
		*/
		virtual void init(PluginContext &_context) throw (...) = 0;
	
		/**
		*	destructor
		*/
		virtual ~IPluginActivator() = 0;

	};
	
	inline IPluginActivator::~IPluginActivator() {}

}

/**
*	
*/
#define _2REAL_EXPORT_PLUGIN(x)\
	POCO_BEGIN_MANIFEST(IPluginActivator)\
	POCO_EXPORT_CLASS(x)\
	POCO_END_MANIFEST