 /*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2012 Fachhochschule Salzburg GmbH

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

#include "ComplexCounterBlock.h"
#include "engine/_2RealData.h"

ComplexCounter::ComplexCounter( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr< _2Real::bundle::AbstractBlock > > const& dependencies ) :
	_2Real::bundle::AbstractBlock( io, dependencies )
{
}

ComplexCounter::~ComplexCounter()
{
}

void ComplexCounter::setup()
{	
	_2Real::bundle::ParameterHandle *parameter = mIo.mParameters[ 0 ];
	_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 0 ];
	boost::get< _2Real::CustomDataItem >( outlet->getValue() ) = boost::get< _2Real::CustomDataItem >( parameter->getValue() );

	//std::cout << boost::get< _2Real::CustomDataItem >( outlet->getValue() ) << std::endl;
}

void ComplexCounter::update()
{
	_2Real::bundle::InletHandle *inlet = dynamic_cast< _2Real::bundle::InletHandle * >( mIo.mInlets[ 0 ] );
	_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 0 ];

	// extract a reference to the simple field in the outlet data
	_2Real::CustomDataItem & counter = boost::get< _2Real::CustomDataItem >( outlet->getValue() );
	_2Real::CustomDataItem & val_out = counter.getValue< _2Real::CustomDataItem >( "simple_field" );

	// get const& ref to inlet data
	_2Real::CustomDataItem const& val_in = boost::get< _2Real::CustomDataItem >( inlet->getValue() );

	/*
	*	here, boost::apply_visitor might come in handy...
	*/
	val_out.getValue< uint8_t >( "uchar_field" ) += val_in.getValue< uint8_t >( "uchar_field" );
	val_out.getValue< uint32_t >( "uint_field" ) += val_in.getValue< uint32_t >( "uint_field" );
	val_out.getValue< uint64_t >( "ulong_field" ) += val_in.getValue< uint64_t >( "ulong_field" );
	val_out.getValue< float >( "float_field" ) += val_in.getValue< float >( "float_field" );
	val_out.getValue< double >( "double_field" ) += val_in.getValue< double >( "double_field" );

	std::cout << counter << std::endl;
}

void ComplexCounter::shutdown()
{
}