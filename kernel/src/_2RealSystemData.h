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

#include "_2RealXMLReader.h"
#include "_2RealSystem.h"

#include <string>
#include <list>

namespace _2Real
{

	class Symbol
	{

	public:

		Symbol(std::string const& name);
		virtual ~Symbol();

		/**
		*	returns the name
		*/
		std::string const& getName() const;

		/**
		*	essentially, sets the id
		*/
		void resolveTo(Identifier const& id);

		/**
		*	returns the id
		*/
		Identifier const& getId() const;

		/**
		*	true if id is valid
		*/
		const bool isResolved() const;

	private:

		std::string			const m_Name;
		Identifier			m_Id;

	};

	class ExtendedSymbol : public Symbol
	{

	public:

		ExtendedSymbol(std::string const& extendedName, std::string const& symbolName);
		std::string const& getExtendedName() const;

	private:

		std::string			const m_ExtendedName;

	};

	class ParamValue
	{

	public:

		ParamValue(std::string const& name, std::string const& value);
		std::string const& getName() const;
		std::string const& getValue() const;
		
	private:

		std::string		const m_Name;
		std::string		const m_Value;

	};

	class IOLink
	{

	public:

		IOLink(std::string const& serviceIn, std::string const& input, std::string const& serviceOut, std::string const& output);

		std::string const& getInServiceName() const;
		std::string const& getInSlotName() const;
		std::string const& getOutServiceName() const;
		std::string const& getOutSlotName() const;
		Identifier const& getInputId() const;
		Identifier const& getOutputId() const;

		void resolveInputService(Identifier const& serviceId);
		void resolveOutputService(Identifier const& serviceId);

		const bool isFullyResolved() const;

	private:

		ExtendedSymbol		m_In;
		ExtendedSymbol		m_Out;

	};

	class GraphData : public Symbol
	{

	public:

		GraphData(std::string const& name, float fps, std::string const& type);
		virtual ~GraphData();

		const float getFps() const;
		std::string const& getType() const;

		std::list< GraphData * > & getChildren();

		virtual const bool isFullyResolved() const;

	private:

		float						const m_Fps;
		std::string					const m_Type;

		std::list< GraphData * >	m_Children;
		//std::set< ExtendedSymbol >	m_Slots;

	};

	class ServiceData : public GraphData
	{

	public:

		ServiceData(std::string const& name, float fps, std::string const& service, std::string const& plugin);

		std::list< ParamValue > & getSetupInfo();

		std::string const& getServiceName() const;
		std::string const& getPluginName() const;
		void resolvePlugin(Identifier const& id);
		Identifier const& getPluginId() const;

		const bool isFullyResolved() const;

	private:

		ExtendedSymbol				m_Service;

		std::list< ParamValue >		m_SetupInfo;

	};

	class PluginData : public Symbol
	{

	public:

		PluginData(std::string const& name, std::string const& classname);

		std::string const& getClassname() const;
		std::list< ParamValue > & getSetupInfo();

	private:

		std::string					const m_Classname;
		std::list< ParamValue >		m_SetupInfo;

	};

	class SystemData
	{

	public:

		SystemData(std::string const& xmlPath);
		~SystemData();

		const std::string getDefaultDirectory() const;
		const std::string getLogfile() const;

		std::list< PluginData > & getPlugins();
		std::list< GraphData * > & getGraphs();
		std::list< IOLink > & getIOLinks();

	private:

		void processSystem();
		void processPluginList(Poco::XML::Node const& plugins, std::list< PluginData > &pluginList, std::string const& prefix);
		void processParameterList(Poco::XML::Node const& params, std::list< ParamValue > &paramList, std::string const& prefix);
		void processGraphList(Poco::XML::Node const& graphs, std::list< GraphData * > &graphList, std::string const& prefix);
		void processIOLinkList(Poco::XML::Node const& links, std::list< IOLink > &linkList, std::string const& prefix);
		GraphData *processExternalGraph(std::string const& name, float fps, std::string const& source, std::string const& prefix);

		std::string					m_SystemName;
		std::string					m_DefaultDir;
		std::string					m_Logfile;

		std::list< PluginData >		m_Plugins;
		std::list< GraphData * >	m_Graphs;
		std::list< IOLink >			m_IOLinks;

		std::set< std::string >		m_Names;

		XMLReader					m_Reader;

	};

}