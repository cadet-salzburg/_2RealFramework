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

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "_2RealImagebuffer.h"

#include <iostream>

template< typename ImageData >
static _2Real::IService *const createFloatConversion(void)
{
	_2Real::IService *service = new FloatConversionService< ImageData >();
	return service;
}

/**
*	converts an pixelbuffer< ImageData > to a pixelbuffer< float >
*/

template< typename ImageData >
class FloatConversionService : public _2Real::IService
{

public:

	FloatConversionService();
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~FloatConversionService();

private:

	_2Real::InputHandle			m_Input;
	_2Real::OutputHandle		m_Output;

};

template< typename ImageData >
FloatConversionService< ImageData >::~FloatConversionService()
{
}

template< typename ImageData >
FloatConversionService< ImageData >::FloatConversionService() :
	IService()
{
}

template< typename ImageData >
void FloatConversionService< ImageData >::setup(_2Real::ServiceContext &context)
{
	try
	{
		//cout << "FloatConversionService: setup " << endl;

		m_Input = context.getInputHandle("input image");
		m_Output = context.getOutputHandle("output image");

		//cout << "FloatConversionService: setup finished" << endl;
	}
	catch (Exception &e)
	{
		throw e;
	}
	catch (...)
	{
		throw ServiceException("FloatConversionService: unexpected error in setup");
	}
};

template< typename ImageData >
void FloatConversionService< ImageData >::update()
{
	try
	{
		//cout << "FloatConversionService: update " << endl;

		Pixelbuffer< ImageData > const& newImage(m_Input.getNewestData< Pixelbuffer < ImageData > >());
		Pixelbuffer< float > & outImage(m_Output.data< Pixelbuffer < float > >());

		unsigned int size = newImage.size();

		ImageData const *const in = newImage.rawData();
		float *out = new float[size];

		for (unsigned int i=0; i<size; i++)
		{
			out[i] = float(in[i]);
		}

		outImage.assign(out, newImage.width(), newImage.height(), newImage.channels(), true);

		//cout << "FloatConversionService: update finished " << endl;
	}
	catch (Exception &e)
	{
		throw e;
	}
	catch (...)
	{
		throw ServiceException("FloatConversionService: unexpected error in update");
	}
};