#pragma once

#include "_2realblock.h"

#include "ShapeWrapDeviceManager.h"

/*
class ShapeWrapBlock

Defines the ShapeWrapput Block --> Via a ShapeWrap Input Port the ShapeWrap Messages are sent through three Outlets.
*/
class ShapeWrapBlock : public _2Real::bundle::Block
{
public:
	ShapeWrapBlock( _2Real::bundle::ContextBlock & context );
	~ShapeWrapBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	void							discardAllOutlets();

	// The Block Inlets 
	_2Real::bundle::InletHandle	m_ShapeWrapPortInlet;
	_2Real::bundle::InletHandle m_iBaudRateInlet;
	_2Real::bundle::InletHandle m_ReadIntel;

	// The Block Outlets
	_2Real::bundle::OutletHandle m_WriteOutlet;
	
	// The ShapeWrapDevice Manager
	ShapeWrapDeviceManager*		m_ShapeWrapDeviceManager;
};
