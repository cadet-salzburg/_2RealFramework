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

namespace _2Real
{
	// cat = important for auto conversion and the like
	class TypeCategory
	{

	public:

		enum Category
		{
			ARITHMETHIC,
			LOGICAL,
			CONTAINER,			// vector / list / string / image
			UNIQUE,				// everything else
			EIGEN_TYPE			// stuff taken from the eigen lib
		};

		TypeCategory( Category const& cat ) : m_Category( cat ) {}
		Category getCategory() const { return m_Category; }
		bool operator==( TypeCategory const& rhs ) const { return m_Category == rhs.m_Category; }
		bool operator!=( TypeCategory const& rhs ) const { return m_Category != rhs.m_Category; }

	private:

		Category		m_Category;

	};
}