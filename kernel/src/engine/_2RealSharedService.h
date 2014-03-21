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

#include "bundle/_2RealAbstractBlock.h"

namespace _2Real
{

	typedef _2Real::bundle::AbstractBlock AbstractSharedService;

	//class AbstractSharedService
	//{
	//public:
	//	virtual ~AbstractSharedService() {};
	//	virtual void setup() = 0;
	//	virtual void update() = 0;
	//	virtual void shutdown() = 0;
	//};

	//template< typename TType >
	//class SharedService_T : public AbstractSharedService
	//{
	//public:

	//	SharedService_T() { mObject = new TType; }
	//	~SharedService_T() { delete mObject; }

	//	void setup() { mObject->setup(); }
	//	void update() { mObject->update(); }
	//	void shutdown() { mObject->shutdown(); }

	//private:

	//	TType					*mObject;

	//};

}