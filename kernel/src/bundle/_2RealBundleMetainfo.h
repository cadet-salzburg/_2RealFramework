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

#include "common/_2RealStdIncludes.h"

namespace _2Real
{
	class BundleMetainfoImpl;
	class InletDeclaration;
	class OutletDeclaration;
	class ParameterDeclaration;
	class FieldDeclaration;

	namespace bundle
	{
		class BundleMetainfo
		{

		public:

			explicit BundleMetainfo( std::shared_ptr< BundleMetainfoImpl > );

			// setters for basic bundle info
			void setAuthor( const std::string );
			void setDescription( const std::string );
			void setContact( const std::string  );
			void setCategory( const std::string );
			void setVersion( const unsigned int, const unsigned int, const unsigned int );

			void exportsType( const std::string name, const std::vector< FieldDeclaration > fields );
			// removed 'singleton' attrib, may be re-added if necessary
			void exportsBlock( const std::string name, const std::vector< InletDeclaration > inlets, const std::vector< OutletDeclaration > outlets, const std::vector< ParameterDeclaration > parameters );

		private:

			std::shared_ptr< BundleMetainfoImpl >		mImpl;

		};
	}
}