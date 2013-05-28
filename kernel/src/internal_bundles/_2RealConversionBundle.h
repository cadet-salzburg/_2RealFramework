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

#include "datatypes/_2RealImage.h"
#include "datatypes/_2RealImageT.h"
#include "datatypes/_2RealTypes.h"
#include "../_2RealBundle.h"
#include "internal_bundles/_2RealInternalBundles.h"

namespace _2Real
{

	//class AbstractConversionBlock : public bundle::Block
	//{

	//public:

	//	virtual ~AbstractConversionBlock() {}

	//	void setup( bundle::BlockHandle &block )
	//	{
	//		m_Block = block;
	//		m_Src = block.getInletHandle( "src" );
	//		m_Dst = block.getOutletHandle( "dst" );
	//	}

	//	void shutdown() {}

	//protected:

	//	bundle::BlockHandle			m_Block;
	//	bundle::InletHandle			m_Src;
	//	bundle::OutletHandle		m_Dst;

	//};

	//template< typename TSrc, typename TDst >
	//class ConversionBlock;

	//template< >
	//class ConversionBlock< Image8U, Image > : public AbstractConversionBlock
	//{
	//	void update()
	//	{
	//		Image8U const& src = m_Src.getReadableRef< Image8U >();
	//		Image &dst = m_Dst.getWriteableRef< Image >();

	//		const unsigned char w = src.getWidth();
	//		const unsigned char h = src.getHeight();
	//		const unsigned char c = src.getNumberOfChannels();
	//		const unsigned int sz = w*h*c;
	//		unsigned char *data = new unsigned char[ sz ];

	//		memcpy( data, src.getData(), sz );

	//		dst.assign( data, true, src.getWidth(), src.getHeight(), src.getChannelOrder() );
	//	}
	//};

	//template< >
	//class ConversionBlock< Image16U, Image > : public AbstractConversionBlock
	//{
	//	void update()
	//	{
	//		Image16U const& src = m_Src.getReadableRef< Image16U >();
	//		Image &dst = m_Dst.getWriteableRef< Image >();

	//		const unsigned char w = src.getWidth();
	//		const unsigned char h = src.getHeight();
	//		const unsigned char c = src.getNumberOfChannels();
	//		const unsigned int sz = w*h*c;
	//		unsigned short *data = new unsigned short[ sz ];

	//		memcpy( data, src.getData(), sz*sizeof( unsigned short ) );

	//		dst.assign( data, true, src.getWidth(), src.getHeight(), src.getChannelOrder() );
	//	}
	//};

	//template< >
	//class ConversionBlock< Image32F, Image > : public AbstractConversionBlock
	//{
	//	void update()
	//	{
	//		Image32F const& src = m_Src.getReadableRef< Image32F >();
	//		Image &dst = m_Dst.getWriteableRef< Image >();

	//		const unsigned char w = src.getWidth();
	//		const unsigned char h = src.getHeight();
	//		const unsigned char c = src.getNumberOfChannels();
	//		const unsigned int sz = w*h*c;
	//		float *data = new float[ sz ];

	//		memcpy( data, src.getData(), sz*sizeof( float ) );

	//		dst.assign( data, true, src.getWidth(), src.getHeight(), src.getChannelOrder() );
	//	}
	//};

	//template< >
	//class ConversionBlock< Image64F, Image > : public AbstractConversionBlock
	//{
	//	void update()
	//	{
	//		Image64F const& src = m_Src.getReadableRef< Image64F >();
	//		Image &dst = m_Dst.getWriteableRef< Image >();

	//		const unsigned char w = src.getWidth();
	//		const unsigned char h = src.getHeight();
	//		const unsigned char c = src.getNumberOfChannels();
	//		const unsigned int sz = w*h*c;
	//		double *data = new double[ sz ];

	//		memcpy( data, src.getData(), sz*sizeof( double ) );

	//		dst.assign( data, true, src.getWidth(), src.getHeight(), src.getChannelOrder() );
	//	}
	//};

	//template< typename TSrc, typename TDst >
	//std::string getConversionName()
	//{
	//	TypeDescriptor *tSrc = createTypeDescriptor< TSrc >();
	//	TypeDescriptor *tDst = createTypeDescriptor< TDst >();

	//	std::string src = tSrc->m_TypeName;
	//	std::string dst = tDst->m_TypeName;

	//	for ( unsigned int i = 0; i<src.length(); ++i )
	//	{
	//		if ( src[i] == ' ' ) src[i] = '_';
	//	}
	//	for ( unsigned int i = 0; i<dst.length(); ++i )
	//	{
	//		if ( dst[i] == ' ' ) dst[i] = '_';
	//	}

	//	delete tDst;
	//	delete tSrc;

	//	std::string conversion = src + "_to_" + dst;
	//	return conversion;
	//}

	//template< typename TSrc, typename TDst >
	//void addConversion( bundle::BundleMetainfo &info )
	//{
	//	bundle::BlockMetainfo &b = info.exportBlock< ConversionBlock< TSrc, TDst >, bundle::WithoutContext >( getConversionName< TSrc, TDst >() );
	//	b.addInlet< TSrc >( "src", TSrc() );
	//	b.addOutlet< TDst >( "dst" );
	//}

	//inline void getMetainfoForConversions( bundle::BundleMetainfo &info )
	//{
	//	info.setName( strTypeConversions );
	//	info.setAuthor( "cadet team" );
	//	info.setDescription( "type conversions" );
	//	info.setCategory( "internal" );
	//	info.setVersion( 0, 1, 0 );

	//	addConversion< Image8U, Image >( info );
	//	addConversion< Image16U, Image >( info );
	//	addConversion< Image32F, Image >( info );
	//	addConversion< Image64F, Image >( info );

	//	//addConversion< Image, Image8U >( info );
	//	//addConversion< Image, Image16U >( info );
	//	//addConversion< Image, Image32F >( info );
	//	//addConversion< Image, Image64F >( info );
	//}
}