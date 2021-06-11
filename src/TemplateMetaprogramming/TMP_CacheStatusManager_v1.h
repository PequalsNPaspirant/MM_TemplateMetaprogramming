//Goal
//Create a task container and executor identified by string/enum

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>
#include <mutex>
#include <condition_variable>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace cacheStatusManager_v1 {

		template<typename T> struct is_shared_ptr : std::false_type {};
		template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
		template<typename T> struct is_shared_ptr<std::shared_ptr<const T>> : std::true_type {};

		class Hashable
		{
		public:
			virtual size_t hash() const = 0;
		};

		template<typename KeyType>
		class Hash
		{
		public:
			size_t operator()(const KeyType& key) const
			{
				return key.hash();
			}
		};

		std::string getThreadId()
		{
			std::thread::id id = std::this_thread::get_id();
			std::stringstream ss;
			ss << id;
			return ss.str();
		}

		template<typename... Args>
		void log(const std::string& level, Args... args)
		{
			std::stringstream ss;
			ss << "\n" << level << " [" << getThreadId() << "] ";
			using expander = int[];
			(void)expander {
				0, (void(ss << ',' << std::forward<Args>(args)), 0)...
			};

			std::cout << ss.str();
		}

		template<
			typename KeyType, typename ValueType,
			typename Hasher = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType>,
			typename std::enable_if<is_shared_ptr<ValueType>::value>::type* = nullptr>
		class Cache
		{
		public:
			void set(const KeyType& key, ValueType value)
			{
				log("INFO", "Cache: Setting data in cache: key, value: ", key, value);
				std::unique_lock<std::mutex> lock{ mu_ };
				cache_[key] = value;
			}

			ValueType get(const KeyType& key)
			{
				log("INFO", "Cache: Getting data from cache: key: ", key);
				std::unique_lock<std::mutex> lock{ mu_ };
				auto it = cache_.find(key);
				if (it != cache_.end())
					return it->second;

				return nullptr;
			}

			void clear()
			{
				log("INFO", "Cache: Clearing cache");
				std::unique_lock<std::mutex> lock{ mu_ };
				cache_.clear();
			}

		private:
			std::unordered_map<KeyType, ValueType, Hasher, KeyEqual> cache_;
			std::mutex mu_;
		};

		template<
			//template<typename... Args> typename CacheType,
			typename CacheType,
			typename KeyType, typename ValueType,
			typename Hasher = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType>,
			typename std::enable_if<is_shared_ptr<ValueType>::value>::type* = nullptr>
		class CacheStatusManager
		{
		public:
			CacheStatusManager(CacheType& cache) : cache_{ cache } {}
			~CacheStatusManager() = default;

		private:
			//No syncronization is required for this class
			class WriteAccess
			{
			public:
				WriteAccess(CacheStatusManager& cacheStatusMgr, const KeyType& key, bool accessGranted) :
					cacheStatusMgr_{ cacheStatusMgr },
					key_{ key },
					accessGranted_{ accessGranted }
				{}

				~WriteAccess()
				{
					if (accessGranted_)
					{
						//If the status is still not 'available', probably there was exception in data preparing function, 
						// status may be still 'preparing', change the status and notify all threads
						//If the status is not 'preparing', some other thread might have timed out and changed status
						log("CRIT", "WriteAccess: ~WriteAccess: data is still not cached");
						cacheStatusMgr_.resetStatusIfStillPreparing(key_);
					}
				}

				//Do not allow copy
				WriteAccess(const WriteAccess&) = delete;
				WriteAccess& operator=(const WriteAccess&) = delete;
				//Allow move
				WriteAccess(WriteAccess&&) = default;
				WriteAccess& operator=(WriteAccess&&) = default;

				void set(ValueType value)
				{
					if (!accessGranted_)
					{
						//This may happen when the data preparing thread took long time than timeout, so another thread timedout and it changed status
						log("CRIT", "WriteAccess: set: forcefully setting data in cache: key, value: ", key_, value);
					}

					cacheStatusMgr_.set(key_, value);
				}

				bool accessGranted() { return accessGranted_; }

			private:
				CacheStatusManager& cacheStatusMgr_;
				KeyType key_;
				bool accessGranted_{ false };
			};

		public:
			template<typename Fun, typename... Args>
			ValueType setOrGet(const KeyType& key, int numRetries, bool wait, size_t timeoutMilliSec, Fun funToCreateValue, Args... args)
			{
				ValueType value;
				for (int tries = 0; !value && tries < numRetries; ++tries) //if this thread timed out, try again for numRetries times
				{
					log("INFO", "CacheStatusManager: setOrGet: tries: ", tries);
					WriteAccess wa = getWriteAccess(key);
					if (wa.accessGranted())
					{
						value = funToCreateValue(forward<Args>(args)...); //this function may throw
						wa.set(value);
					}
					else //wait for cache to be updated
						value = get(key, wait, timeoutMilliSec);
				}

				return value;
			}

			void clear()
			{
				log("INFO", "CacheStatusManager: clear: ");
				std::unique_lock<std::mutex> lock{ muStatus_ };
				cacheStatus_.clear();
				lock.unlock();

				cache_.clear();

				cvStatus_.notify_all(); //in case any thread is still waiting on cv. (THIS SHOULD NOT HAPPEN)
			}

		private:
			WriteAccess getWriteAccess(const KeyType& key)
			{
				std::unique_lock<std::mutex> lock{ muStatus_ };

				CacheStatus& status = cacheStatus_[key];
				if (status == CacheStatus::unavailable)
				{
					log("INFO", "CacheStatusManager: getWriteAccess: access granted: key: ", key);
					status = CacheStatus::preparing;
					return WriteAccess{ *this, key, true };
				}
				else
					return WriteAccess{ *this, key, false };
			}

			ValueType get(const KeyType& key, bool wait, size_t timeoutMilliSec)
			{
				std::unique_lock<std::mutex> lock{ muStatus_ };
				CacheStatus& status = cacheStatus_[key];

				std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
				while (status == CacheStatus::preparing)
				{
					if (!wait)
						return nullptr;

					log("INFO", "CacheStatusManager: get: waiting: key: ", key);

					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					std::chrono::milliseconds elaspedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					std::chrono::milliseconds remainingTimeout = std::chrono::milliseconds{ timeoutMilliSec } -elaspedTime;
					if (remainingTimeout.count() <= 0 || cvStatus_.wait_for(lock, remainingTimeout) == std::cv_status::timeout)
					{
						log("CRIT", "CacheStatusManager: get: timed out: key: ", key);
						if (status != CacheStatus::available) //If no other thread has yet pushed data in cache
						{
							status = CacheStatus::unavailable;
							lock.unlock();
							//The thread which was preparing data could not finish within time limit, reason may be exception, infinite loop etc.
							//So, notify all waiting threads to be on safer side.
							cvStatus_.notify_all();
							return nullptr;
						}
					}
				}

				if(status == CacheStatus::available)
					return cache_.get(key);

				return nullptr;
			}

			void set(const KeyType& key, ValueType value)
			{
				//update the cache before updating the status
				cache_.set(key, value);

				std::unique_lock<std::mutex> lock{ muStatus_ };
				CacheStatus& status = cacheStatus_[key];
				status = CacheStatus::available;
			}

			void resetStatusIfStillPreparing(const KeyType& key)
			{
				std::unique_lock<std::mutex> lock{ muStatus_ };
				CacheStatus& status = cacheStatus_[key];
				if (status == CacheStatus::preparing)
					status = CacheStatus::unavailable;
			}

			enum class CacheStatus
			{
				unavailable = 0,
				preparing,
				available
			};

			CacheType& cache_;
			std::unordered_map<KeyType, CacheStatus, Hasher, KeyEqual> cacheStatus_;
			std::mutex muStatus_;
			std::condition_variable cvStatus_;
		};

	}

}

