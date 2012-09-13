#pragma once

#include "_2RealBlock.h"

namespace Poco
{
	namespace Net
	{
		class DatagramSocket;
	}
}

class OpenBeaconBlock : public _2Real::bundle::Block
{
public:

	OpenBeaconBlock();
	virtual ~OpenBeaconBlock();
	virtual void shutdown();
	virtual void update();
	virtual void setup( _2Real::bundle::BlockHandle &context );

private:

	virtual void discardAllOutlets();

	_2Real::bundle::BlockHandle		m_Context;

	_2Real::bundle::InletHandle		m_UDPPort;
	_2Real::bundle::OutletHandle	m_Tags;

	Poco::Net::DatagramSocket*		m_pSocket;
};