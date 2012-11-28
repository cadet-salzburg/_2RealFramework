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

#include "_2RealDatatypes.h"
#include <math.h>

namespace _2Real
{
	namespace gl
	{
		class Arcball
		{

		public:
	
			Arcball() :
				mInitialMousePos( 0, 0 ), mCenter( 0, 0 ),
				mCurrentQuat( Eigen::Quaternionf::Identity() ), mInitialQuat( Eigen::Quaternionf::Identity() ),
				mRadius( 0.f ), mConstrainToAxis( false ), mConstraintAxis( 0.f, 1.f, 0.f )
			{
			}

			void mouseDown( const unsigned int x, const unsigned int y )
			{
				mInitialMousePos = Eigen::Vector2i( x, y );
				mInitialQuat = mCurrentQuat;
			}

			void mouseDrag( const unsigned int x, const unsigned int y )
			{
				Eigen::Vector3f from = mouseOnSphere( mInitialMousePos );
				Eigen::Vector2i v( x, y );
				Eigen::Vector3f to = mouseOnSphere( v );

				if ( mConstrainToAxis )
				{
					from = constrainToAxis( from, mConstraintAxis );
					to = constrainToAxis( to, mConstraintAxis );
				}
		
				Eigen::Vector3f axis = from.cross( to );
				mCurrentQuat = mInitialQuat * Eigen::Quaternionf( from.dot( to ), axis.x(), axis.y(), axis.z() );
				mCurrentQuat.normalize();
			}

			void resetRotation() { mCurrentQuat = mInitialQuat = Eigen::Quaternionf::Identity(); }

			Eigen::Matrix4f getRotation() const
			{
				Eigen::Matrix3f mat3 = mCurrentQuat.toRotationMatrix();
				Eigen::Matrix4f mat4 = Eigen::Matrix4f::Identity();

				for ( unsigned int i=0; i<3; ++i )
				{
					for ( unsigned int j=0; j<3; ++j )
					{
						unsigned int m3 = i*3+j;
						unsigned int m4 = i*4+j;

						mat4( m4 ) = mat3( m3 );
					}
				}

				return mat4;
			}

			void					setCenter( const float cx, const float cy ) { mCenter = Eigen::Vector2f( cx, cy ); }
			void					setRadius( const float r ) { mRadius = r; }
			Eigen::Vector2f			getCenter() const { return mCenter; }
			float					getRadius() const { return mRadius; }
	
			Eigen::Vector3f mouseOnSphere( Eigen::Vector2i const& point ) const
			{
				Eigen::Vector3f result;
		
				result.x() = ( point.x() - mCenter.x() ) / ( mRadius * 2.f );
				result.y() = ( point.y() - mCenter.y() ) / ( mRadius * 2.f );
				result.z() = 0.f;

				float mag = result.squaredNorm();
				if ( mag > 1.0f ) result.normalize();
				else 
				{
					result.z() = sqrt( 1.0f - mag );
					result.normalize();
				}

				return result;
			}

			void setConstraintAxis( float x, float y, float z ) { mConstraintAxis = Eigen::Vector3f( x, y, z ); }
			void enableConstraintAxis( const bool use = true ) { mConstrainToAxis = use; }
			bool isConstraintAxisEnabled() const { return mConstrainToAxis; }
	
		private:

			static Eigen::Vector3f constrainToAxis( Eigen::Vector3f const& loose, Eigen::Vector3f const& axis )
			{
				Eigen::Vector3f onPlane = loose - axis * axis.dot( loose );
				float norm = onPlane.squaredNorm();

				if( norm > 0.f ) 
				{
					if( onPlane.z() < 0.f ) onPlane = -onPlane;
					return ( onPlane * ( 1.f / sqrt( norm ) ) );
				}
		
				if( axis.dot( Eigen::Vector3f( 0.f, 1.f, 0.f ) ) < 0.0001f )
				{
					onPlane = Eigen::Vector3f( 1.f, 0.f, 0.f );
				}
				else
				{
					onPlane = Eigen::Vector3f( -axis.y(), axis.x(), 0.0f ).normalized();
				}
		
				return onPlane;
			}

			Eigen::Vector2i		mInitialMousePos;
			Eigen::Vector2f		mCenter;
			Eigen::Quaternionf	mCurrentQuat;
			Eigen::Quaternionf	mInitialQuat;
			float				mRadius;
			bool				mConstrainToAxis;
			Eigen::Vector3f		mConstraintAxis;

		};
	}
}