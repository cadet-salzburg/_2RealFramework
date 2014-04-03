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

void ComplexCounter::getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, std::map< std::string, const _2Real::bundle::TypeMetainfo > const& types )
{
	_2Real::bundle::FunctionBlockMetainfo &counterinfo = dynamic_cast< _2Real::bundle::FunctionBlockMetainfo & >( info );

	const _2Real::bundle::TypeMetainfo simpleInfo = types.at( "simpleType" );
	const _2Real::bundle::TypeMetainfo complexInfo = types.at( "complexType" );

	counterinfo.setBlockClass< ComplexCounter >();
	counterinfo.setDescription( "counter" );
	//counterinfo.setDefaultUpdatePolicy( _2Real::bundle::VALUES_NEW( _2Real::bundle::VALUES_NEW::ANY ) );

	_2Real::bundle::InletMetainfo in = counterinfo.getInletMetainfo( "increment" );
	in.setDescription( "counter increment" );
	in.setMultiInlet( false );
	in.setDatatypeAndInitialValue( simpleInfo.makeData() );

	_2Real::bundle::ParameterMetainfo param = counterinfo.getParameterMetainfo( "init" );
	param.setDescription( "counter initial value" );
	param.setDatatypeAndInitialValue( complexInfo.makeData() );

	_2Real::bundle::OutletMetainfo out = counterinfo.getOutletMetainfo( "value" );
	out.setDescription( "counter current value" );
	out.setDatatypeAndInitialValue( complexInfo.makeData() );
}

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