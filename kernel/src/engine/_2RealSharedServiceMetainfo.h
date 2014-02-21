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

namespace _2Real
{
	class SharedServiceInletMetainfo;
	class SharedServiceOutletMetainfo;
	class DefaultPolicy;
	class AbstractSharedServiceFactory;

	class SharedServiceMetainfo : public std::enable_shared_from_this< SharedServiceMetainfo >
	{

	public:

		SharedServiceMetainfo( std::string const& name );
		~SharedServiceMetainfo();

		void setSingleton( const bool );
		void setDescription( std::string const& );
		void setDependencies( std::vector< std::string > const& );

		std::shared_ptr< SharedServiceInletMetainfo > createInlet( std::string const& name );
		std::shared_ptr< SharedServiceOutletMetainfo > createOutlet( std::string const& name );
		std::shared_ptr< SharedServiceInletMetainfo > createParameter( std::string const& name );

		void setDefaultUpdatePolicy( std::shared_ptr< DefaultPolicy > );
		void setDefaultSetupPolicy( std::shared_ptr< DefaultPolicy > );

		bool isSingleton() const;
		std::string const&		getName() const;
		std::string const&		getDescription() const;
		void getDependencies( std::vector< std::string > & ) const;

		void getInletMetainfos( std::vector< std::shared_ptr< const SharedServiceInletMetainfo > > & ) const;
		void getOutletMetainfos( std::vector< std::shared_ptr< const SharedServiceOutletMetainfo  > > & ) const;
		void getParameterMetainfos( std::vector< std::shared_ptr< const SharedServiceInletMetainfo  > > & ) const;

		void getDefaultSetupPolicy( std::vector< std::vector< std::string > > & ) const;
		void getDefaultUpdatePolicy( std::vector< std::vector< std::string > > & ) const;

		void setFactory( std::shared_ptr< AbstractSharedServiceFactory > factory );
		std::shared_ptr< const AbstractSharedServiceFactory > getFactory() const;

	private:

		SharedServiceMetainfo( SharedServiceMetainfo const& );
		SharedServiceMetainfo& operator=( SharedServiceMetainfo const& );

		typedef std::map< std::string, std::shared_ptr< SharedServiceInletMetainfo > >		Inlets;
		typedef std::map< std::string, std::shared_ptr< SharedServiceOutletMetainfo > >		Outlets;

		std::string					mName;
		std::string					mDescription;
		std::vector< std::string >	mDependencies;
		bool						mIsSingleton;

		std::shared_ptr< DefaultPolicy >					mDefaultSetupPolicy;
		std::shared_ptr< DefaultPolicy >					mDefaultUpdatePolicy;

		std::shared_ptr< AbstractSharedServiceFactory >		mFactory;

		Inlets				mInlets;
		Inlets				mParameters;
		Outlets				mOutlets;

	};

}