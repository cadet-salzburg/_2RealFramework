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

#include "common/_2RealData.h"
#include "common/_2RealDeclarations.h"
#include "common/_2RealException.h"

#include "bundle/_2RealBlock.h"

#include "bundle/_2RealBundleMetainfo.h"
#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealTypeMetainfo_I.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "bundle/_2RealCustomTypeMetainfo.h"
#include "bundle/_2RealTypeMetainfoCollection.h"
#include "bundle/_2RealIoSlotMetainfo.h"
#include "bundle/_2RealUpdatePolicyMetainfo.h"

#include "bundle/_2RealBlockIo.h"
#include "bundle/_2RealInletHandle_I.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealMultiInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "bundle/_2RealParameterHandle.h"

#include "engine/_2RealHumanReadableNameVisitor.h"

#if defined( _WIN32 )
	#define _2REAL_LIBRARY_API __declspec( dllexport )
#else
	#define _2REAL_LIBRARY_API
#endif

extern "C"	/* used to prevent name mangling under windows */
{
	void _2REAL_LIBRARY_API getBundleMetainfo( _2Real::bundle::BundleMetainfo & info );
	void _2REAL_LIBRARY_API getTypeMetainfo( _2Real::bundle::CustomTypeMetainfo & info, _2Real::bundle::TypeMetainfoCollection const& types );
	void _2REAL_LIBRARY_API getBlockMetainfo(  _2Real::bundle::BlockMetainfo & info, _2Real::bundle::TypeMetainfoCollection const& types );
}