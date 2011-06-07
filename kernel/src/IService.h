#pragma once

/*
	every service that wants to be registered
	needs to implement this
*/

namespace _2Real
{
	class IService
	{
	public:
		typedef IService* const ConstPtr;
	};
}