 /*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2012 Fachhochschule Salzburg GmbH

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

#include "_2RealBundle.h"

namespace zmq
{
	class context_t;
	class socket_t;
};

class Publisher : public _2Real::bundle::Block
{

public:

	static void getBlockMetainfo( _2Real::bundle::BlockMetainfo &, _2Real::bundle::TypeMetainfoCollection const& );

	Publisher( _2Real::bundle::BlockIo const&, std::vector< std::shared_ptr< _2Real::bundle::Block > > const& );
	~Publisher();

	void setup();
	void update();
	void shutdown();

private:

	// TODO: ctxt manager?
	std::unique_ptr< zmq::context_t	>	mContext;
	std::unique_ptr< zmq::socket_t >	mSocket;

};
