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

#include <set>

namespace _2Real
{

	class InletIO;
	class OutletIO;

	class IOLink
	{

	public:

		IOLink( InletIO &inlet, OutletIO &outlet );

		void activate();
		void deactivate();

		InletIO const& getInletIO() const;
		OutletIO const& getOutletIO() const;

		bool operator<( IOLink const& other );


	private:

		InletIO		&m_InletIO;
		OutletIO	&m_OutletIO;

	};

}