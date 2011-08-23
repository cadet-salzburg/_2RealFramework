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
	*	exception system is incomplete
	*	if you don't mind me, i'll talk to myself here for a bit.
	*
	*	internally used exceptions: stuff that helps me to debug shit
	*	copy exception - if i copy an an obj that i don't want to copy b/c it has no copy ctor (same goes for assignment)
	*							internally used only
	*	invalid id exception:	
	*
	*
	*
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