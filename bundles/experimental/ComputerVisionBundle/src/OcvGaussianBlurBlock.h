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

#include "_2RealBlock.h"

class OcvGaussianBlurBlock : public _2Real::bundle::Block
{

public:

	OcvGaussianBlurBlock();
	virtual ~OcvGaussianBlurBlock();
	virtual void shutdown();
	virtual void update();
	virtual void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle			m_Block;
	unsigned int						m_OutWidth;
	unsigned int						m_OutHeight;
	_2Real::ImageType					m_OutImageType;
	_2Real::ImageChannelOrder			m_OutChannelOrder;

};

class OcvHistogramEqualizationBlock : public _2Real::bundle::Block
{

public:

	OcvHistogramEqualizationBlock();
	virtual ~OcvHistogramEqualizationBlock();
	virtual void shutdown();
	virtual void update();
	virtual void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle			m_Block;
	unsigned int						m_OutWidth;
	unsigned int						m_OutHeight;
	_2Real::ImageType					m_OutImageType;
	_2Real::ImageChannelOrder			m_OutChannelOrder;

};