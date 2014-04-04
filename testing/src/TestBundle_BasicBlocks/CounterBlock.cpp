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

#include "CounterBlock.h"

void Counter::getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, std::map< std::string, const _2Real::bundle::TypeMetainfo > const& previousTypes )
{
	_2Real::bundle::FunctionBlockMetainfo &counterinfo = dynamic_cast< _2Real::bundle::FunctionBlockMetainfo & >( info );

	counterinfo.setBlockClass< Counter >();
	counterinfo.setDescription( "test block - some simple counters" );

	_2Real::bundle::DefaultUpdatePolicy policy = counterinfo.getDefaultUpdatePolicy();
	policy.set( _2Real::DefaultPolicy::ALL );

	_2Real::bundle::InletMetainfo inA = counterinfo.getInletMetainfo( "inA" );
	inA.setDescription( "int increment" );
	inA.setMultiInlet( false );
	inA.setDatatypeAndInitialValue( ( int32_t )10 );

	_2Real::bundle::InletMetainfo inB = counterinfo.getInletMetainfo( "inB" );
	inB.setDescription( "float increment" );
	inB.setMultiInlet( false );
	inB.setDatatypeAndInitialValue( 1.f );

	_2Real::bundle::InletMetainfo inC = counterinfo.getInletMetainfo( "inC" );
	inC.setDescription( "ulong increment" );
	inC.setMultiInlet( false );
	inC.setDatatypeAndInitialValue( ( uint64_t )1000 );

	_2Real::bundle::ParameterMetainfo paramA = counterinfo.getParameterMetainfo( "paramA" );
	paramA.setDescription( "int init value" );
	paramA.setDatatypeAndInitialValue( ( int32_t )0 );

	_2Real::bundle::ParameterMetainfo paramB = counterinfo.getParameterMetainfo( "paramB" );
	paramB.setDescription( "float init value" );
	paramB.setDatatypeAndInitialValue( 0.f );

	_2Real::bundle::ParameterMetainfo paramC = counterinfo.getParameterMetainfo( "paramC" );
	paramC.setDescription( "ulong init value" );
	paramC.setDatatypeAndInitialValue( ( uint64_t )0 );

	_2Real::bundle::OutletMetainfo outA = counterinfo.getOutletMetainfo( "outA" );
	outA.setDescription( "int counter" );
	outA.setDatatypeAndInitialValue( ( int32_t )0 );

	_2Real::bundle::OutletMetainfo outB = counterinfo.getOutletMetainfo( "outB" );
	outB.setDescription( "float counter" );
	outB.setDatatypeAndInitialValue( 0.f );

	_2Real::bundle::OutletMetainfo outC = counterinfo.getOutletMetainfo( "outC" );
	outC.setDescription( "ulong counter" );
	outC.setDatatypeAndInitialValue( ( uint64_t )0 );
}

Counter::Counter( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr< _2Real::bundle::AbstractBlock > > const& dependencies ) :
	_2Real::bundle::AbstractBlock( io, dependencies )
{
}

Counter::~Counter()
{
}

void Counter::setup()
{	
	{

		_2Real::bundle::ParameterHandle *parameter = mIo.mParameters[ 0 ];
		_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 0 ];

		int32_t& val = boost::get< int32_t >( outlet->getValue() );
		int32_t const& init = boost::get< int32_t >( parameter->getValue() );

		val = init;
		std::cout << "  int val " << val << std::endl;

	}

	{

		_2Real::bundle::ParameterHandle *parameter = mIo.mParameters[ 1 ];
		_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 1 ];

		float & val = boost::get< float >( outlet->getValue() );
		float const& init = boost::get< float >( parameter->getValue() );

		val = init;
		std::cout << "float val " << val << std::endl;

	}

	{

		_2Real::bundle::ParameterHandle *parameter = mIo.mParameters[ 2 ];
		_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 2 ];

		uint64_t & val = boost::get< uint64_t >( outlet->getValue() );
		uint64_t const& init = boost::get< uint64_t >( parameter->getValue() );

		val = init;
		std::cout << "ulong val " << val << std::endl;

	}
}

void Counter::update()
{
	{

		_2Real::bundle::InletHandle *inlet = dynamic_cast< _2Real::bundle::InletHandle * >( mIo.mInlets[ 0 ] );
		_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 0 ];

		int32_t & val = boost::get< int32_t >( outlet->getValue() );
		int32_t const& inc = boost::get< int32_t >( inlet->getValue() );

		val += inc;
		std::cout << "  int val " << val << std::endl;

	}

	{

		_2Real::bundle::InletHandle *inlet = dynamic_cast< _2Real::bundle::InletHandle * >( mIo.mInlets[ 1 ] );
		_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 1 ];

		float & val = boost::get< float >( outlet->getValue() );
		float const& inc = boost::get< float >( inlet->getValue() );

		val += inc;
		std::cout << "float val " << val << std::endl;

	}

	{

		_2Real::bundle::InletHandle *inlet = dynamic_cast< _2Real::bundle::InletHandle * >( mIo.mInlets[ 2 ] );
		_2Real::bundle::OutletHandle *outlet = mIo.mOutlets[ 2 ];

		uint64_t & val = boost::get< uint64_t >( outlet->getValue() );
		uint64_t const& inc = boost::get< uint64_t >( inlet->getValue() );

		val += inc;
		std::cout << "ulong val " << val << std::endl;

	}
}

void Counter::shutdown()
{
}