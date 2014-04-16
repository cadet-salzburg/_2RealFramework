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

#include "common/_2RealStdIncludes.h"

namespace _2Real
{
	class FieldDeclaration
	{
	public:
		FieldDeclaration( const std::string, const bool );
		std::string mName;
		bool		mIsCustom;
	};

	class InletDeclaration
	{
	public:
		InletDeclaration( const std::string, const bool );
		std::string mName;
		bool		mIsMulti;
	};

	class OutletDeclaration
	{
	public:
		OutletDeclaration( const std::string );
		std::string mName;
		//bool		mIsMulti;
	};

	class ParameterDeclaration
	{
	public:
		ParameterDeclaration( const std::string );
		std::string mName;
		//bool		mIsMulti;
	};

	class TypeDeclaration
	{
	public:
		TypeDeclaration() = default;
		std::string mName;
		std::vector< FieldDeclaration >	mFields;
	};

	class BlockDeclaration
	{
	public:
		BlockDeclaration() = default;
		std::string mName;
		bool mIsSingleton;
		std::vector< InletDeclaration > mInlets;
		std::vector< OutletDeclaration > mOutlets;
		std::vector< ParameterDeclaration > mParameters;
	};

	FieldDeclaration declareCustomField( const std::string );
	FieldDeclaration declareBasicField( const std::string );
	InletDeclaration declareInlet( const std::string );
	InletDeclaration declareMultiInlet( const std::string );
	OutletDeclaration declareOutlet( const std::string );
	ParameterDeclaration declareParameter( const std::string );

}