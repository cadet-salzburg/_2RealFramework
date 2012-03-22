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

#include "_2RealImageT.h"

namespace _2Real
{

	ImageChannelOrder::ImageChannelOrder(const CHANNEL_CODE channelCode) :
		m_ChannelCode(channelCode)
	{
		switch(m_ChannelCode)
		{
			case RGBA:
				set(0, 1, 2, 3, 4);
			break;
			case BGRA:
				set(2, 1, 0, 3, 4);
			break;
			case ARGB:
				set(1, 2, 3, 0, 4);
			break;
			case ABGR:
				set(3, 2, 1, 0, 4);
			break;
			case RGBX:
				set(0, 1, 2, INVALID, 4);
			break;
			case BGRX:
				set(2, 1, 0, INVALID, 4);
			break;
			case XRGB:
				set(1, 2, 3, INVALID, 4);
			break;
			case XBGR:
				set(3, 2, 1, INVALID, 4);
			break;
			case RGB:
				set(0, 1, 2, INVALID, 3);
			break;
			case BGR:
				set(2, 1, 0, INVALID, 3);
			break;
			case R:
				set(0, INVALID, INVALID, INVALID, 1);
			break;
			case G:
				set(INVALID, 0, INVALID, INVALID, 1);
			break;
			case B:
				set(INVALID, INVALID, 0, INVALID, 1);
			break;
			case A:
				set(INVALID, INVALID, INVALID, 0, 1);
			break;
		}
	}

	ImageChannelOrder::ImageChannelOrder(ImageChannelOrder const& src) :
		m_RedOffset(src.m_RedOffset),
		m_GreenOffset(src.m_GreenOffset),
		m_BlueOffset(src.m_BlueOffset),
		m_AlphaOffset(src.m_AlphaOffset),
		m_ChannelCode(src.m_ChannelCode),
		m_NumChannels(src.m_NumChannels)
	{
	}

	void ImageChannelOrder::set(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha, const uint8_t channelCount)
	{
		m_RedOffset = red;
		m_GreenOffset = green;
		m_BlueOffset = blue;
		m_AlphaOffset = alpha;
		m_NumChannels = channelCount;
	}

}