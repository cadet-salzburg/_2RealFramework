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

#include "_2RealApplication.h"
#include "app/_2RealEngine.h"
#include "Poco/SharedPtr.h"
#include <limits>
#include <map>

namespace _2Real
{
	class ConfigurationLoader;

	namespace app
	{
		template< typename V, typename E >
		class Edge_t;

		template< typename V, typename E >
		class Vertex_t;

		template< typename V, typename E >
		class Edge_t
		{

		public:

			typedef Vertex_t< V, E >	VertexType;

			Edge_t( E const& content, VertexType &vStart, VertexType &vEnd ) : mContent( content )
			{
				const unsigned int idIn = vStart.mEdges.size();
				const unsigned int idOut = vEnd.mEdges.size();
				mVertices = std::make_pair( Incidence( idIn, vStart ), Incidence( idOut, vEnd ) );
				vStart.mEdges.push_back( this );
				vEnd.mEdges.push_back( this );
			}

			bool operator<( Edge_t< V, E > const& other ) const
			{
				return ( mContent < other.mContent );
			}

			struct Incidence
			{
				Incidence() : index( 0 ), vertex( nullptr ) {}
				Incidence( const unsigned int i, VertexType &v ) : index( i ), vertex( &v ) {}
				unsigned int		index;
				VertexType			*vertex;
			};

			std::pair< Incidence, Incidence >	mVertices;
			E									mContent;

		};

		template< typename V, typename E >
		struct EdgeComp_t
		{
			bool operator()( Edge_t< V, E > const *const e1, Edge_t< V, E > const *const e2 ) const
			{
				return ( *e1 < *e2 );
			}
		};

		template< typename V, typename E >
		class Vertex_t
		{

		public:

			typedef Edge_t< V, E >									EdgeType;
			typedef std::vector< Edge_t< V, E > * >					Edges;

			Vertex_t( V const& content ) : mEdges(), mContent( content ) {}

			Edges		mEdges;
			V			mContent;

		};

		class SystemState
		{

		public:

			typedef Edge_t< app::BlockHandle, app::Engine::Link >		Edge;
			typedef Vertex_t< app::BlockHandle, app::Engine::Link >		Vertex;

			SystemState() {}
			//SystemState& operator=( SystemState const& other );
			//SystemState( SystemState const& other );
			//~SystemState();

			//SystemState * difference( SystemState const& other ) const;

			void getBlockHandles( std::list< app::BlockHandle > &l ) const
			{
				l.clear();

				for ( VertexConstIter vIter = mVertices.begin(); vIter != mVertices.end(); ++vIter )
				{
					l.push_back( vIter->second->mContent );
				}
			}

		private:

			friend class app::Engine;
			friend class _2Real::ConfigurationLoader;

			typedef std::set< Edge *, EdgeComp_t< app::BlockHandle, app::Engine::Link > >			Edges;
			typedef std::set< Edge *, EdgeComp_t< app::BlockHandle, app::Engine::Link > >::iterator EdgeIter;

			Edges			mEdges;

			typedef std::map< app::BlockHandle, Vertex * >					Vertices;
			typedef std::map< app::BlockHandle, Vertex * >::iterator		VertexIter;
			typedef std::map< app::BlockHandle, Vertex * >::const_iterator	VertexConstIter;

			Vertices		mVertices;

		};
	}
}