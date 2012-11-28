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
#include <iostream>
#include <string>
#include <sstream>
#include "KinectOpenNIDepthBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void KinectOpenNIDepthBlock::setup( BlockHandle &block )
{
	// set generator type so base class object works on that generator, this inheritance helps avoid useless codeduplicatio, you could use templates as well
	setGeneratorType(_2RealKinectWrapper::DEPTHIMAGE);
	KinectOpenNIBlockBase::setup(block);
	m_IsAlignedToColorInletHandle = block.getInletHandle("IsAlignedToColor");
	m_Is16BitInletHandle = block.getInletHandle("Is16BitImage");
	m_IsRealWorldInletHandle = block.getInletHandle("IsPointCloud");
	m_bIsRealWorld = m_IsRealWorldInletHandle.getReadableRef<bool>();
	m_bIsAlignedToColor = m_IsAlignedToColorInletHandle.getReadableRef<bool>();
}

void KinectOpenNIDepthBlock::update()
{
	try
	{
		if(m_OpenNIDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no cameras connected there is nothing todo so return
		{
			m_WidthOutletHandle.discard();
			m_HeightOutletHandle.discard();
			m_ImageOutletHandle.discard();
			return;
		}

		bool bIsAlignedToColor = m_IsAlignedToColorInletHandle.getReadableRef<bool>();
		if( m_iCurrentDevice!=-1 && bIsAlignedToColor != m_bIsAlignedToColor)
		{
			m_OpenNIDeviceManager->setAlignToColor(m_iCurrentDevice, bIsAlignedToColor);
			m_bIsAlignedToColor = bIsAlignedToColor;
		}

		bool bIs16Bit = m_Is16BitInletHandle.getReadableRef<bool>();
		if( bIs16Bit != m_bIs16Bit)
		{
			m_bIs16Bit = bIs16Bit;
		}

		bool bIsRealWorld = m_IsRealWorldInletHandle.getReadableRef<bool>();
		if( bIsRealWorld != m_bIsRealWorld )
		{
			m_bIsRealWorld = bIsRealWorld;
		}

		// call update of base class
		KinectOpenNIBlockBase::update();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}


void KinectOpenNIDepthBlock::updateImageOutlet()
{
	if ( m_bIsRealWorld )
	{
		Image &inImg = m_OpenNIDeviceManager->getImage( m_iCurrentDevice, m_GeneratorType, true );

		unsigned short * inIter = reinterpret_cast< unsigned short * >( inImg.getData() );

		const unsigned int w = inImg.getWidth();
		const unsigned int h = inImg.getHeight();
		const unsigned int sz = w*h;

		_2RealKinectWrapper::_2RealVector3f *proj = new _2RealKinectWrapper::_2RealVector3f[ sz ];
		_2RealKinectWrapper::_2RealVector3f *projIter = proj;

		for ( unsigned int i=0; i<h; ++i )
		{
			for ( unsigned int j=0; j<w; ++j )
			{
				*projIter = _2RealKinectWrapper::_2RealVector3f( float( j ), float( i ), float( *inIter ) );
				++projIter;
				++inIter;
			}
		}

		_2RealKinectWrapper::_2RealVector3f *world = new _2RealKinectWrapper::_2RealVector3f[ sz ];
		m_OpenNIDeviceManager->projectiveToReal( m_iCurrentDevice, sz, proj, world );

		delete [] proj;

		float *out = new float[ 3*sz ];
		float *outIter = out;

		_2RealKinectWrapper::_2RealVector3f *worldIter = world;

		for ( unsigned int i=0; i<sz; ++i )
		{
			*outIter = worldIter->x / 1000.0f; ++outIter;
			*outIter = worldIter->y / 1000.0f; ++outIter;
			*outIter = worldIter->z / 1000.0f; ++outIter;
			++worldIter;
		}

		delete [] world;

		//float *out = static_cast< float * >( static_cast< void * >( world ) );

		Image &outImg = m_ImageOutletHandle.getWriteableRef<_2Real::Image >();
		// TODO: sth goes terribly wrong with assign
		//outImg.assign( out, true, w, h, ImageChannelOrder::RGB );
		outImg = Image( out, true, w, h, ImageChannelOrder::RGB );
	}
	else
	{
		if( m_bIs16Bit )	m_ImageOutletHandle.getWriteableRef<_2Real::Image >() = m_OpenNIDeviceManager->getImage( m_iCurrentDevice, m_GeneratorType, true );
		else				m_ImageOutletHandle.getWriteableRef<_2Real::Image >() = m_OpenNIDeviceManager->getImage( m_iCurrentDevice, m_GeneratorType );
	}
}