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

#include "Poco/Path.h"

#include <string>
#include <list>

namespace _2Real
{

	class SymbolName : public std::list< std::string >
	{

	public:

		SymbolName(std::string const& name);
		SymbolName(std::list< std::string > const& symbols);
		SymbolName();

		const std::string toString() const;

		const SymbolName getPrefixes() const;
		const std::string getLastSymbol() const;
		void addPrefixes(SymbolName symbols);

		void pushSymbol(std::string const& symbol);
		void popSymbol();
		void popPrefix();

	};

	class Symbol
	{

	public:

		Symbol(SymbolName const& name);
		virtual ~Symbol();

		/**
		*	returns the name
		*/
		SymbolName const& getName() const;

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

	protected:

		SymbolName			const m_Name;
		Identifier			m_Id;

	};

	class ExtendedSymbol : public Symbol
	{

	public:

		ExtendedSymbol(SymbolName const& name);
		const std::string getExtendedName() const;
		const SymbolName getSymbolName() const;

	};

	class ParamValue
	{

	public:

		ParamValue(SymbolName const& name, std::string const& value);
		SymbolName const& getName() const;
		std::string const& getValue() const;
		
	private:

		SymbolName		const m_Name;
		std::string		const m_Value;

	};

	class IOLink
	{

	public:

		IOLink(SymbolName const& in, SymbolName const& out);

		const SymbolName getInServiceName() const;
		const std::string getInSlotName() const;
		const SymbolName getOutServiceName() const;
		const std::string getOutSlotName() const;
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

		GraphData(SymbolName const& name, float fps, std::string const& type);
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

	/**
	*	this renames an existing parameter (or slot) to another name
	*/
	class ParameterRedefinition : public ExtendedSymbol
	{

	};

	class ServiceData : public GraphData
	{

	public:

		ServiceData(SymbolName const& name, float fps, SymbolName const& service);

		std::list< ParamValue > & getSetupInfo();

		const std::string getServiceName() const;
		const SymbolName getPluginName() const;
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

		PluginData(SymbolName const& name, std::string const& classname, Poco::Path const& path);

		Poco::Path const& getPluginPath() const;
		std::string const& getClassname() const;
		std::list< ParamValue > & getSetupInfo();

	private:

		std::string					const m_Classname;
		Poco::Path					const m_Path;
		std::list< ParamValue >		m_SetupInfo;

	};

	class XMLData
	{

	public:

		XMLData(Poco::Path const& path, SymbolName const& name);
		virtual ~XMLData();

		std::list< PluginData > & getPlugins();
		std::list< GraphData * > & getGraphs();
		std::list< IOLink > & getIOLinks();

		std::set< SymbolName > & getNames();

	protected:

		void processPluginList(Poco::XML::Node const& plugins, std::list< PluginData > &pluginList, SymbolName &prefix);
		void processParameterList(Poco::XML::Node const& params, std::list< ParamValue > &paramList, SymbolName &prefix);
		void processGraphList(Poco::XML::Node const& graphs, std::list< GraphData * > &graphList, SymbolName &prefix, bool replace = false);
		void processIOLinkList(Poco::XML::Node const& links, std::list< IOLink > &linkList, SymbolName &prefix, bool replace = false);

		SymbolName					const m_NamePrefix;

		std::list< PluginData >		m_Plugins;
		std::list< GraphData * >	m_Graphs;
		std::list< IOLink >			m_IOLinks;

		std::set< SymbolName >		m_Names;

		XMLReader					m_Reader;

	};

	class SystemData : public XMLData
	{

	public:

		SystemData(Poco::Path const& path);

		const Poco::Path getDefaultDirectory() const;
		const std::string getLogfile() const;

	private:

		void processSystem();

		Poco::Path					m_DefaultDir;
		std::string					m_Logfile;

	};

	class BlockData : public XMLData
	{

	public:

		BlockData(Poco::Path const& path, SymbolName const& name);

	private:

		void processBlock();

	};

}