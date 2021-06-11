//Goal
//Create a task container and executor identified by string/enum

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>
#include <thread>
#include <random>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "TMP_CacheStatusManager_v1.h"

namespace mm {

	namespace cacheStatusManager_v1 {

		struct MyKey : public Hashable
		{
			MyKey(int val) : val_{ val } {}
			MyKey(const MyKey&) = default;
			MyKey(MyKey&&) = default;

			size_t hash() const override
			{
				return std::hash<int>{}(val_);
			}

			bool operator==(const MyKey& rhs) const
			{
				return val_ == rhs.val_;
			}

			friend ostream& operator<<(ostream& o, const MyKey& key)
			{
				o << key.val_;
				return o;
			}

			int val_;
		};

		struct MyValue
		{
			MyValue(int val) : val_{ val } {}

			friend ostream& operator<<(ostream& o, const MyValue& value)
			{
				o << value.val_;
				return o;
			}

			friend ostream& operator<<(ostream& o, std::shared_ptr<MyValue> value)
			{
				o << value->val_;
				return o;
			}

			int val_;
		};

		void testCacheStatusManager()
		{
			using CacheType = Cache<MyKey, std::shared_ptr<MyValue>, Hash<MyKey>>;
			using CacheStatusMgrType = CacheStatusManager<CacheType, MyKey, std::shared_ptr<MyValue>, Hash<MyKey>>;
			auto threadFunction = [](CacheStatusMgrType& cacheStatusMgr, int numIterations, size_t timeoutMilliSec) {

				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_int_distribution<int> distKey(1, numIterations);
				std::uniform_int_distribution<size_t> distTimeout(1, timeoutMilliSec / 2);

				auto prepareDataToCache = [timeoutMilliSec](int val, size_t currentTimeout) {

					//randomly delay to make other threads time out
					if (val % 10 == 0)
						std::this_thread::sleep_for(std::chrono::milliseconds{ timeoutMilliSec + 50 });
					else
						std::this_thread::sleep_for(std::chrono::milliseconds{ currentTimeout });

					//randomly throw
					if (val % 10 == 1)
						throw std::runtime_error{ "destructive testing" };

					//very randomly go into infinite loop
					if (val % 1000 == 0)
					{
						log("CRIT", "============================== Going into infinite loop: key: ", val);
						while(true)
							std::this_thread::sleep_for(std::chrono::milliseconds{ currentTimeout });
					}

					return std::make_shared<MyValue>(val);
				};

				int numRetries = 3;
				bool wait = true;

				for (int iter = 0; iter < numIterations; ++iter)
				{
					//generate random key
					MyKey key{ distKey(mt) };
					size_t currentTimeout{ distTimeout(mt) };
					try
					{
						std::shared_ptr<MyValue> val = cacheStatusMgr.setOrGet(key, numRetries, wait, timeoutMilliSec, prepareDataToCache, key.val_, currentTimeout);
					}
					catch (...)
					{
						log("CRIT", "Exception: key: ", key);
					}
				}
			};

			int numThreads = 200;
			int numIterations = 5000;
			size_t timeoutMilliSec = 200;

			CacheType cache;
			CacheStatusMgrType cacheMgr{ cache };

			std::vector<std::thread> threadPool;
			for (int i = 0; i < numThreads; ++i)
				threadPool.push_back(std::thread{ threadFunction, std::ref(cacheMgr), numIterations, timeoutMilliSec });

			for (int i = 0; i < numThreads; ++i)
				if (threadPool[i].joinable())
					threadPool[i].join();

			cacheMgr.clear();
		}
	}


	MM_DECLARE_FLAG(TMPCacheStatusManager_v1);

	MM_UNIT_TEST(TMPCacheStatusManager_v1_Test, TMPCacheStatusManager_v1)
	{
		cacheStatusManager_v1::testCacheStatusManager();
	}
}

