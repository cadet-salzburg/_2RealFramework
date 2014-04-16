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

#include "_2RealBundle.h"
#include "PrintInfoBlock.h"
#include "CounterBlock.h"

void getBundleMetainfo( _2Real::bundle::BundleMetainfo &info )
{
	info.setAuthor( "fhs33223" );
	info.setDescription( "a test bundle that exports simple blocks only - no context blocks and no types" );
	info.setCategory( "testing" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	info.exportsBlock( "printInfoBlock",  { _2Real::declareInlet( "inA" ), _2Real::declareInlet( "inB" ), _2Real::declareInlet( "inC" ) }, { _2Real::declareOutlet( "outA" ), _2Real::declareOutlet( "outB" ) }, { _2Real::declareParameter( "paramA" ), _2Real::declareParameter( "paramB" ) } );
	info.exportsBlock( "counterBlock", { _2Real::declareInlet( "inA" ), _2Real::declareInlet( "inB" ), _2Real::declareInlet( "inC" ) }, { _2Real::declareOutlet( "outA" ), _2Real::declareOutlet( "outB" ), _2Real::declareOutlet( "outC" ) }, { _2Real::declareParameter( "paramA" ), _2Real::declareParameter( "paramB" ), _2Real::declareParameter( "paramC" ) } );
}

void getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, std::vector< const _2Real::bundle::CustomTypeMetainfo > const& allTypes )
{
	if ( info.getName() == "printInfoBlock" )
		PrintInfo::getBlockMetainfo( info, allTypes );
	else if ( info.getName() == "counterBlock" )
		Counter::getBlockMetainfo( info, allTypes );
}