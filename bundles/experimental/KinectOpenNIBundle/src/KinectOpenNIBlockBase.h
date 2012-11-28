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
#include "_2RealBlock.h"
#include "OpenNIDeviceManager.h"

using namespace _2Real::bundle;

class KinectOpenNIBlockBase : public Block
{
public:
					KinectOpenNIBlockBase( ContextBlock & context );
	virtual			~KinectOpenNIBlockBase();
	virtual void	shutdown();
	virtual void	update();
	virtual void	updateImageOutlet();
	virtual void	setup( BlockHandle &context );
	void			setGeneratorType(_2RealKinectWrapper::_2RealGenerator generatorType);

protected:
	void									discardAllOutlets();

	InletHandle								m_DeviceIndexInletHandle;
	InletHandle								m_WidthInletHandle;
	InletHandle								m_HeightInletHandle;
	InletHandle								m_FpsInletHandle;
	InletHandle								m_IsMirroredInletHandle;
	OutletHandle							m_ImageOutletHandle;
	OutletHandle							m_WidthOutletHandle;
	OutletHandle							m_HeightOutletHandle;
	OutletHandle							m_FovHorizontalHandle;
	OutletHandle							m_FovVerticalHandle;
	OpenNIDeviceManager*					m_OpenNIDeviceManager;
	_2RealKinectWrapper::_2RealGenerator	m_GeneratorType;
	double									m_dFovH;
	double									m_dFovV;
	int										m_iCurrentDevice;
	int										m_iWidth;
	int										m_iHeight;
	int										m_iFps;
	bool									m_bIsMirrored;
};