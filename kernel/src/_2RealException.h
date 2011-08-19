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

	/**
	*	placeholder class thingie, since it's not really decided if we're going to use
	*	exceptions or return codes. 
	*	right now, i throw these objects in some functions, while using them as return
	*	values in others Oo
	*/

	class Exception
	{

	public:

		Exception();

		Exception(unsigned int const& _nr);

		unsigned int m_Code;

		static Exception			s_Success;
		static Exception			s_Failure;
		static Exception			s_NoCopy;
		static Exception const&	success();
		static Exception const&	failure();
		static Exception const&	noCopy();

		bool operator==(Exception const& _rhs);
		bool operator!=(Exception const& _rhs);
		Exception& operator=(Exception const& _src);

	};

}