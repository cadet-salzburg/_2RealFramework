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

#include "datatypes/_2RealCustomData.h"
#include <vector>

namespace _2Real
{
	class Image : public CustomType
	{

	public:

		Image();
		Image& operator=( Image const& other );
		bool operator==( Image const& other ) const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		void set( std::vector< unsigned char > const& data, const unsigned int w, const unsigned int h );

	};

	template< >
	struct Init< Image >
	{
		static Image defaultValue()
		{
			return Image();
		}
	};

	template< >
	struct Name< Image >
	{
		static std::string humanReadableName()
		{
			return "image";
		}
	};
}