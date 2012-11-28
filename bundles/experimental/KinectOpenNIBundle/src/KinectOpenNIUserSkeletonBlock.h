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
#include "KinectOpenNIBlockBase.h"

using namespace _2Real::bundle;

class KinectOpenNIUserSkeletonBlock : public KinectOpenNIBlockBase
{
public:
	KinectOpenNIUserSkeletonBlock( ContextBlock & context ) : KinectOpenNIBlockBase(context)
	{
	};
	
	void					setup( BlockHandle &context );
	void					update();

private:
	InletHandle				m_IsAlignedToColorInletHandle;
	InletHandle				m_IsWorldCoordinatesInletHandle;
	OutletHandle			m_SkeletonsOutletHandle;
	OutletHandle			m_NrOfUsersOutletHandle;
	OutletHandle			m_NrOfSkeletonsOutletHandle;
	OutletHandle			m_UsersCenterOfMass;
	int						m_iNrOfUsers;
	int						m_iNrOfSkeletons;
	bool					m_bIsAlignedToColor;
	bool					m_bIsWorldCoordinates;
};