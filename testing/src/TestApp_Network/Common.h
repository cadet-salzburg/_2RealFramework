#pragma once

// will be renamed...

#include "_2RealApplication.h"
#include <deque>

namespace _2Real
{
	namespace network
	{
		class ReceiverQueue
		{

		public:

			ReceiverQueue( ReceiverQueue const& ) = delete;
			ReceiverQueue( ReceiverQueue && ) = delete;

			ReceiverQueue() = default;
			~ReceiverQueue() = default;

			typedef boost::signals2::signal< void() >::slot_type OverflowCallback;
			enum class OverflowPolicy : uint8_t { DROP_NEWEST, DROP_OLDEST };

			explicit ReceiverQueue( const uint32_t maxSize, OverflowCallback cb = nullptr );

			void add( std::shared_ptr< const DataItem > );
			std::shared_ptr< const DataItem > getNewest();
			std::shared_ptr< const DataItem > getOldest();
			bool empty() const;
			uint32_t size() const;

		private:

			std::deque< std::shared_ptr< const _2Real::DataItem > >		mData;
			mutable std::mutex											mLock;

		};

		inline void ReceiverQueue::add( std::shared_ptr< const DataItem > data )
		{
			std::lock_guard< std::mutex > lock( mLock );
			mData.push_front( data );
		}

		inline std::shared_ptr< const DataItem > ReceiverQueue::getNewest()
		{
			std::lock_guard< std::mutex > lock( mLock );
			auto result = mData.front(); mData.pop_front();
			return result;
		}

		inline std::shared_ptr< const DataItem > ReceiverQueue::getOldest()
		{
			std::lock_guard< std::mutex > lock( mLock );
			auto result = mData.back(); mData.pop_back();
			return result;
		}

		inline bool ReceiverQueue::empty() const		
		{
			std::lock_guard< std::mutex > lock( mLock );
			return mData.empty();
		}

		inline uint32_t ReceiverQueue::size() const
		{
			std::lock_guard< std::mutex > lock( mLock );
			return mData.size();
		}
	}
}