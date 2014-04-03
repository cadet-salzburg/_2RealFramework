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

	info.exportBlock( "printInfoBlock", false, { "inA", "inB", "inC" }, { "outA", "outB" }, { "paramA", "paramB" } );
	info.exportBlock( "counterBlock", false, { "inA", "inB", "inC" }, { "outA", "outB", "outC" }, { "paramA", "paramB", "paramC" } );
}

void getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, std::map< std::string, const _2Real::bundle::TypeMetainfo > const& previousTypes )
{
	if ( info.getName() == "printInfoBlock" )
		PrintInfo::getBlockMetainfo( info, previousTypes );
	else if ( info.getName() == "counterBlock" )
		Counter::getBlockMetainfo( info, previousTypes );
}