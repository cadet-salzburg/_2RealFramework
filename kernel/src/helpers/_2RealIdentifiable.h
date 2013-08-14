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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealConstants.h"

namespace _2Real
{
	class IdGenerator;

	class TemplateId
	{
	public:
		std::string const& getFullHumanReadableName() const { return mFullName; }
		std::string const& getHumanReadableName() const { return mName; }
		std::string const& getObjectName() const { return mObjectName; }
		std::string const& getParentObjectName() const { return mParent->getObjectName(); }
	protected:
		friend class IdGenerator;
		std::shared_ptr< const TemplateId > mParent;
		std::string		mFullName;
		std::string		mName;
		std::string		mObjectName;
	};

	class InstanceId
	{
	public:
		std::string const& getFullHumanReadableName() const { return mFullName; }
		std::string const& getHumanReadableName() const { return mName; }
		std::string const& getObjectName() const { return mTemplate->getObjectName(); }
		std::string const& getParentInstanceObjectName() const { return mParent->getObjectName(); }
		std::string const& getFullTemplateName() const { return mTemplate->getFullHumanReadableName(); }
		std::string const& getTemplateName() const { return mTemplate->getHumanReadableName(); }
		unsigned int getId() const { return mId; }
		int getParentInstanceId() const { if ( mParent != nullptr ) return mParent->getId(); else return -1; }
	protected:
		friend class IdGenerator;
		std::shared_ptr< const TemplateId > mTemplate;
		std::shared_ptr< const InstanceId > mParent;
		std::string		mFullName;
		std::string		mName;
		unsigned int	mId;
	};

	class IdGenerator
	{

	public:

		enum Code
		{
			BUNDLE = 0, EXPORTED_BLOCK = 1, EXPORTED_TYPE = 2, BLOCK_INSTANCE = 3, INLET = 4, OUTLET = 5, PARAMETER = 6, DATA = 7,
		};
		static const std::string sIdStrings[ 8 ];

	// non-instanced

		static std::shared_ptr< TemplateId >  makeBundleId( Poco::Path const& relativePath )
		{
			const std::string fullName = relativePath.toString();
			const std::string file = relativePath.getFileName();
			const std::string name = file.substr( 0, file.length() - Constants::SharedLibrarySuffix.length() );

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = nullptr;
			id->mFullName = fullName;
			id->mName = name;
			id->mObjectName = file;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeExportedBlockId( std::shared_ptr< const TemplateId > bundle, std::string const& blockName )
		{
			const std::string fullName = bundle->getFullHumanReadableName() + "/blocks/" + blockName;
			const std::string name = bundle->getHumanReadableName() + "/b/" + blockName;

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = bundle;
			id->mFullName = fullName;
			id->mName = name;
			id->mObjectName = blockName;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeExportedTypeId(  std::shared_ptr< const TemplateId > bundle, std::string const& typeName )
		{
			const std::string fullName = bundle->getFullHumanReadableName() + "/types/" + typeName;
			const std::string name = bundle->getHumanReadableName() + "/t/" + typeName;

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = bundle;
			id->mFullName = fullName;
			id->mName = name;
			id->mObjectName = typeName;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeFrameworkTypeId( std::string typeName )
		{
			const std::string fullName = Constants::FrameworkTypename + "/types/" + typeName;
			const std::string name = Constants::FrameworkTypename + "/t/" + typeName;

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = nullptr;
			id->mFullName = fullName;
			id->mName = name;
			id->mObjectName = typeName;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeBasicTypeId( std::string typeName )
		{
			const std::string fullName = Constants::BasicTypename + "/types/" + typeName;
			const std::string name = Constants::BasicTypename + "/t/" + typeName;

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = nullptr;
			id->mFullName = fullName;
			id->mName = name;
			id->mObjectName = typeName;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeExportedInletId(  std::shared_ptr< const TemplateId > block, std::string const& inletName )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/inlets/" << inletName;
			std::stringstream name;
			name << block->getHumanReadableName() << "/i/" << name;

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = block;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mObjectName = inletName;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeExportedOutletId(  std::shared_ptr< const TemplateId > block, std::string const& outletName )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/outlets/" << outletName;
			std::stringstream name;
			name << block->getHumanReadableName() << "/o/" << name;

			std::shared_ptr< TemplateId > id( new TemplateId );

			id->mParent = block;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mObjectName = outletName;

			return id;
		}

		static std::shared_ptr< TemplateId >  makeExportedParameterId( std::shared_ptr< const TemplateId > block, std::string const& parameterName )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/parameters/" << parameterName;
			std::stringstream name;
			name << block->getHumanReadableName() << "/p/" << name;

			std::shared_ptr< TemplateId > id ( new TemplateId );

			id->mParent = block;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mObjectName = parameterName;

			return id;
		}

// instanced

		static std::shared_ptr< InstanceId >  makeBlockInstanceId( std::shared_ptr< const TemplateId > block, const unsigned int i )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/instances/" << i;
			std::stringstream name;
			name << block->getHumanReadableName() << "/" << i;

			std::shared_ptr< InstanceId > id( new InstanceId );

			id->mParent = nullptr;
			id->mTemplate = block;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mId = i;

			return id;
		}

		static std::shared_ptr< InstanceId >  makeInletId( std::shared_ptr< const TemplateId > inlet, std::shared_ptr< const InstanceId > block )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/inlets/" << inlet->getObjectName();

			std::stringstream name;
			name << block->getHumanReadableName() << "/i/" << inlet->getObjectName();

			std::shared_ptr< InstanceId > id( new InstanceId );

			id->mParent = block;
			id->mTemplate = inlet;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mId = -1;

			return id;
		}

		static std::shared_ptr< InstanceId >  makeParameterId(  std::shared_ptr< const TemplateId > parameter, std::shared_ptr< const InstanceId > block )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/parameters/" << parameter->getObjectName();

			std::stringstream name;
			name << block->getHumanReadableName() << "/p/" << parameter->getObjectName();

			std::shared_ptr< InstanceId > id( new InstanceId );

			id->mParent = block;
			id->mTemplate = parameter;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mId = -1;

			return id;
		}

		static std::shared_ptr< InstanceId >  makeOutletId(  std::shared_ptr< const TemplateId > outlet, std::shared_ptr< const InstanceId > block )
		{
			std::stringstream fullName;
			fullName << block->getFullHumanReadableName() << "/outlets/" << outlet->getObjectName();

			std::stringstream name;
			name << block->getHumanReadableName() << "/o/" << outlet->getObjectName();

			std::shared_ptr< InstanceId > id( new InstanceId );

			id->mParent = block;
			id->mTemplate = outlet;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mId = -1;

			return id;
		}

		static std::shared_ptr< InstanceId >  makeSubInletId( std::shared_ptr< const InstanceId > inlet, const unsigned int i )
		{
			std::stringstream fullName;
			fullName << inlet->getFullHumanReadableName() << "/sub/" << i;
			std::stringstream name;
			name << inlet->getHumanReadableName() << "/" << i;

			std::shared_ptr< InstanceId > id( new InstanceId );

			id->mParent = inlet;
			id->mTemplate = inlet->mTemplate;
			id->mFullName = fullName.str();
			id->mName = name.str();
			id->mId = i;
		}
	};

}