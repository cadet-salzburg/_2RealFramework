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

#include "datatypes/_2RealSpace.h"

#include <vector>

namespace _2Real
{
	class FeatureDesc
	{
	public:
		FeatureDesc() :
			m_isValid( false )
		{}

		explicit FeatureDesc( const Space2D &region ) :
			m_isValid( true ),
			m_region( region )
		{}

		FeatureDesc( const FeatureDesc &rhs ) :
			m_isValid( rhs.m_isValid ),
			m_region( rhs.m_region )
		{}

		FeatureDesc &operator = ( const FeatureDesc &rhs )
		{
			m_isValid = rhs.m_isValid;
			m_region = rhs.m_region;
			return *this;
		}

		bool operator == ( const FeatureDesc &rhs ) const
		{
			return ( m_isValid == rhs.m_isValid && m_region == rhs.m_region );
		}

		bool isValid() const			{	return m_isValid;	}
		const Space2D &region() const	{	return m_region;	}

	private:
		bool		m_isValid;
		Space2D		m_region;
	};

	class FaceDesc
	{
	public:
		FaceDesc() : 
		  m_faceID( ~0x00 )
		{}

		explicit FaceDesc( unsigned int faceID ) :
			m_faceID( faceID )
		{}

		FaceDesc( unsigned int faceID, const Space2D &faceRegion ) :
			m_faceID( faceID ),
			m_faceRegion( faceRegion )
		{}

		FaceDesc( const FaceDesc &rhs ) :
			m_faceID( rhs.m_faceID ),
			m_faceRegion( rhs.m_faceRegion ),
			m_eyeLeft( rhs.m_eyeLeft ),
			m_eyeRight( rhs.m_eyeRight ),
			m_nose( rhs.m_nose ),
			m_mouth( rhs.m_mouth )
		{}

		FaceDesc &operator = ( const FaceDesc rhs )
		{
			m_faceID = rhs.m_faceID;
			m_faceRegion = rhs.m_faceRegion;

			m_eyeLeft = rhs.m_eyeLeft;
			m_eyeRight = rhs.m_eyeRight;
			m_nose = rhs.m_nose;
			m_mouth = rhs.m_mouth;

			return *this;
		}

		bool operator == ( const FaceDesc &rhs ) const
		{
			return ( 
				m_faceID == rhs.m_faceID &&
				m_faceRegion == rhs.m_faceRegion && 
				m_eyeLeft == rhs.m_eyeLeft && 
				m_eyeRight == rhs.m_eyeRight && 
				m_nose == rhs.m_nose && 
				m_mouth == rhs.m_mouth );
		}

		unsigned int faceID() const				{	return m_faceID;		}
		void faceID( unsigned int faceID )		{	m_faceID = faceID;		}

		const Space2D &faceRegion() const		{	return m_faceRegion;	}

		const FeatureDesc &eyeLeft() const		{	return m_eyeLeft;		}
		void eyeLeft( const FeatureDesc &el )	{	m_eyeLeft = el;			}

		const FeatureDesc &eyeRight() const		{	return m_eyeRight;		}
		void eyeRight( const FeatureDesc &er )	{	m_eyeRight = er;		}

		const FeatureDesc &nose() const			{	return m_nose;			}
		void nose( const FeatureDesc &nose )	{	m_nose = nose;			}

		const FeatureDesc &mouth() const		{	return m_mouth;			}
		void mouth( const FeatureDesc &mouth )	{	m_mouth = mouth;		}

	private:
		unsigned int	m_faceID;
		Space2D			m_faceRegion;

		FeatureDesc		m_eyeLeft;
		FeatureDesc		m_eyeRight;
		FeatureDesc		m_nose;
		FeatureDesc		m_mouth;
	};

	class FaceCast
	{
	public:
		FaceCast() :
			m_faceID( ~0x00 ),
			m_width( 0 ),
			m_height( 0 ),
			m_cntr( 0 )
		{}

		explicit FaceCast( unsigned int faceID, unsigned width, unsigned height ) :
			m_faceID( faceID ),
			m_width( width ),
			m_height( height ),
			m_cntr( 0 )
		{
			this->resize( width, height );
			this->clear();
		}

		FaceCast( unsigned int faceID,
			unsigned int width,
			unsigned int height,
			const std::vector<Vec3> &vertices, 
			const std::vector<Vec3> &normals, 
			const std::vector<unsigned int> &indices ) :
			m_faceID( faceID ),
			m_width( width ),
			m_height( height ),
			m_cntr( 0 ),
			m_vertices( vertices ),
			m_normals( normals ),
			m_indices( indices )
		{
			if( m_vertices.size() != width * height ||
				m_normals.size() != width * height || 
				m_indices.size() != width * height )
				throw std::runtime_error( "FaceCast: size of vectors not valid!" );
		}

		FaceCast( const FaceCast &rhs ) :
			m_faceID( rhs.m_faceID ),
			m_width( rhs.m_width ),
			m_height( rhs.m_height ),
			m_cntr( rhs.m_cntr ),
			m_vertices( rhs.m_vertices ),
			m_normals( rhs.m_normals ),
			m_indices( rhs.m_indices )
		{}

		FaceCast &operator = ( const FaceCast rhs )
		{
			m_faceID = rhs.m_faceID;
			m_width = rhs.m_width;
			m_height = rhs.m_height;

			m_cntr = rhs.m_cntr;

			m_vertices.assign( rhs.m_vertices.begin(), rhs.m_vertices.end() );
			m_normals.assign( rhs.m_normals.begin(), rhs.m_normals.end() );
			m_indices.assign( rhs.m_indices.begin(), rhs.m_indices.end() );

			return *this;
		}

		bool operator == ( const FaceCast &rhs ) const
		{
			if( m_faceID != rhs.m_faceID )
				return false;
			if( m_width != rhs.m_width )
				return false;
			if( m_height != rhs.m_height )
				return false;

			if( m_vertices.size() != rhs.m_vertices.size() )
				return false;
			if( m_normals.size() != rhs.m_normals.size() )
				return false;
			if( m_indices.size() != rhs.m_indices.size() )
				return false;

			for( int i = 0; i < m_vertices.size(); i++ )
				if( m_vertices[i] != rhs.m_vertices[i] )
					return false;

			for( int i = 0; i < m_normals.size(); i++ )
				if( m_normals[i] != rhs.m_normals[i] )
					return false;

			for( int i = 0; i < m_indices.size(); i++ )
				if( m_indices[i] != rhs.m_indices[i] )
					return false;

			return true;
		}

		unsigned int faceID() const				{	return m_faceID;		}
		void faceID( unsigned int faceID )		{	m_faceID = faceID;		}

		unsigned int width()					{	return m_width;			}
		unsigned int height()					{	return m_height;		}

		void clear()
		{
			m_vertices.assign( m_vertices.size(), Vec3( 0.0f, 0.0f, 0.0f ) );
			m_normals.assign( m_normals.size(), Vec3( 0.0f, 0.0f, 0.0f ) );
			m_indices.assign( m_indices.size(), ~0x00 );

			m_cntr = 0;
		}

		void add( const Vec3 &pos, const Vec3 &normal, int index )
		{
			if( m_cntr == m_vertices.size() )
				throw std::runtime_error( "out of bounds in FaceCast::add" );

			m_vertices[m_cntr] = pos;
			m_normals[m_cntr] = normal;
			m_indices[m_cntr] = index;

			m_cntr++;
		}

		void resize( unsigned int width, unsigned int height )
		{
			m_width = width;
			m_height = height;

			unsigned int size = width * height;

			if( m_vertices.size() != size )
				m_vertices.resize( size );
			if( m_normals.size() != size )
				m_normals.resize( size );
			if( m_indices.size() != size )
				m_indices.resize( size );

			if( m_cntr > size )
				m_cntr = size;
		}

		std::vector<Vec3> &getVertices()					{	return m_vertices;	}
		std::vector<Vec3> &getNormals() 					{	return m_normals;	}
		std::vector<unsigned int> &getIndices()				{	return m_indices;	}

		const std::vector<Vec3> &getVertices() const		{	return m_vertices;	}
		const std::vector<Vec3> &getNormals() const			{	return m_normals;	}
		const std::vector<unsigned int> &getIndices() const	{	return m_indices;	}

	private:
		unsigned int	m_faceID;

		unsigned int	m_width;
		unsigned int	m_height;

		unsigned int	m_cntr;

		std::vector<Vec3>			m_vertices;
		std::vector<Vec3>			m_normals;
		std::vector<unsigned int>	m_indices;
	};
}