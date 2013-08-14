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

#include "policies/_2RealUpdatePolicy.h"
#include "datatypes/_2RealCustomData.h"
#include "engine/_2RealTypeMetadata.h"
#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealIdentifiable.h"

namespace _2Real
{
	class IOMetadata
	{

	public:

		enum Sync
		{
			NO_SYNC				= 0x0000,
			SYNC_ON_SETUP		= 0x0001,
			SYNC_ON_UPDATE		= 0x0010,
			SYNC_ON_SHUTDOWN	= 0x0100,
		};

		IOMetadata();

		std::shared_ptr< TemplateId > 					identifier;
		std::shared_ptr< const TypeMetadata >			typeMetadata;
		std::shared_ptr< const CustomType >				initializer;
		bool											canExpand;
		unsigned int									expansionSize;
		bool											canLink;
		bool											isBuffered;
		unsigned int									bufferSize;
		// overflow policy
		bool											canTriggerUpdates;
		UpdatePolicy									updatePolicy;
		unsigned long									synchronizationFlags;

	};
}