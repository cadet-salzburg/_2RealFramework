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

#include "PrintInfoBlock.h"

void PrintInfo::getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, std::map< std::string, const _2Real::bundle::TypeMetainfo > const& previousTypes )
{
	_2Real::bundle::FunctionBlockMetainfo &printinfo = dynamic_cast< _2Real::bundle::FunctionBlockMetainfo & >( info );

	printinfo.setBlockClass< PrintInfo >();
	printinfo.setDescription( "test block - prints the name/value of all io slots in setup, update & shutdown" );

	_2Real::bundle::DefaultUpdatePolicy policy = printinfo.getDefaultUpdatePolicy();
	policy.set( _2Real::DefaultPolicy::ANY );

	_2Real::bundle::InletMetainfo inA = printinfo.getInletMetainfo( "inA" );
	inA.setDescription( "the first inlet" );
	inA.setMultiInlet( false );
	inA.setDatatypeAndInitialValue( ( int32_t )0 );

	_2Real::bundle::InletMetainfo inB = printinfo.getInletMetainfo( "inB" );
	inB.setDescription( "the second inlet" );
	inB.setMultiInlet( false );
	inB.setDatatypeAndInitialValue( 0.1f );

	_2Real::bundle::InletMetainfo inC = printinfo.getInletMetainfo( "inC" );
	inC.setDescription( "the third inlet" );
	inC.setMultiInlet( true );
	inC.setDatatypeAndInitialValue( ( int64_t )pow( 2, 60 ) );

	_2Real::bundle::OutletMetainfo outA = printinfo.getOutletMetainfo( "outA" );
	outA.setDescription( "the first outlet" );
	outA.setDatatypeAndInitialValue( ( bool )false );

	_2Real::bundle::OutletMetainfo outB = printinfo.getOutletMetainfo( "outB" );
	outB.setDescription( "the second outlet" );
	outB.setDatatypeAndInitialValue( ( int32_t )-5 );

	_2Real::bundle::ParameterMetainfo paramA = printinfo.getParameterMetainfo( "paramA" );
	paramA.setDescription( "the first parameter" );
	paramA.setDatatypeAndInitialValue( ( uint32_t )10 );

	_2Real::bundle::ParameterMetainfo paramB = printinfo.getParameterMetainfo( "paramB" );
	paramB.setDescription( "the second parameter" );
	paramB.setDatatypeAndInitialValue( ( uint64_t )0 );
}

PrintInfo::PrintInfo( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr< _2Real::bundle::AbstractBlock > > const& dependencies ) :
	_2Real::bundle::AbstractBlock( io, dependencies )
{
}

PrintInfo::~PrintInfo()
{
}

void PrintInfo::setup()
{		
	for ( auto it : mIo.mParameters )
	{
		std::cout << "parameter " << it->getName() << " : " << it->getValue() << std::endl;
	}
}

void PrintInfo::update()
{
	for ( auto it : mIo.mInlets )
	{
		if ( it->isMultiInlet() )
		{
			_2Real::bundle::MultiInletHandle *inlet = dynamic_cast< _2Real::bundle::MultiInletHandle * >( it );
			std::cout << "inlet " << inlet->getName() << " is a multiinlet, size " << inlet->getSize() << std::endl;
		}
		else
		{
			_2Real::bundle::InletHandle *inlet = dynamic_cast< _2Real::bundle::InletHandle * >( it );
			_2Real::DataItem const& item = inlet->getValue();
			std::cout << "inlet " << inlet->getName() << " : " << item << std::endl;
		}
	}

	for ( auto it : mIo.mOutlets )
	{
		_2Real::DataItem const& item = it->getValue();
		std::cout << "outlet " << it->getName() << " : " << item << std::endl;
	}
}

void PrintInfo::shutdown()
{
}
