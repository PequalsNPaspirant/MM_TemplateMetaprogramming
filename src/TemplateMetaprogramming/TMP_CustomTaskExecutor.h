//Goal
//Create a task container and executor identified by string/enum

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	template<typename T>
	class RetVal_v1
	{
	public:
		RetVal_v1() {}
		//RetVal_v1(const T& val)
		//	: val_{ val }
		//{}

		RetVal_v1(const RetVal_v1&) = default;
		RetVal_v1(RetVal_v1&&) = delete;
		RetVal_v1& operator=(const RetVal_v1&) = delete;
		RetVal_v1& operator=(RetVal_v1&&) = delete;

		T get()
		{
			return val_;
		}

		bool operator==(const RetVal_v1<T>& rhs)
		{
			return val_ == rhs.val_;
		}

		bool operator==(const T& rhsVal)
		{
			return val_ == rhsVal;
		}

		template<typename U, std::enable_if_t<!std::is_same_v<T, U>, void>* dummy = nullptr>
		//bool operator==(const RetVal_v1<U>& rhs)
		bool operator==(const U& rhs) //Ignore all other types than T and RetVal_v1<T>
		{
			return false;
		}

		template<typename F>
		void assign(F fun)
		{
			val_ = fun();
		}

		std::string toString()
		{
			stringstream ss;
			ss << val_;
			return ss.str();
		}

	private:
		T val_;
	};

	template<>
	class RetVal_v1<void>
	{
	public:
		void get()
		{
		}

		template<typename U>
		bool operator==(const U& rhs)
		{
			return true;
		}

		template<typename F>
		void assign(F task)
		{
			task();
		}

		std::string toString()
		{
			return "";
		}
	};

	// =========== CustomTaskExecutor_v1 ===========

	template<typename RetType, typename ExplicitFunType>
	class CustomTaskExecutor_v1
	{
	public:
		static bool addTask(const std::string& name, ExplicitFunType task)
		{
			CustomTaskExecutor_v1& inst = getInstance();
			//std::pair<MapType::iterator, bool> res = inst.taskMap_.insert(MapType::value_type{ name, std::move(task) }); //does not work on gcc
			auto res = inst.taskMap_.insert(typename MapType::value_type( name, std::move(task) ));
			if (!res.second)
				return false;

			return true;
		}

		template<typename... Args>
		static RetType runTask(const std::string& name, Args... args)
		{
			RetVal_v1<RetType> retVal;
			CustomTaskExecutor_v1& inst = getInstance();
			auto it = inst.taskMap_.find(name);
			if (it == inst.taskMap_.end())
				return retVal.get();

			return it->second(args...);
		}

		template<typename... Args>
		static RetType runTaskNoThrow(bool& success, const std::string& name, Args... args)
		{
			RetVal_v1<RetType> retVal;
			try
			{
				retVal.assign([=]() { return runTask(name, args...); });
				success = true;
			}
			catch (...)
			{
				success = false;
			}

			return retVal.get();
		}

	private:
		static CustomTaskExecutor_v1& getInstance()
		{
			static CustomTaskExecutor_v1 inst;
			return inst;
		}

		using MapType = std::unordered_map<std::string, ExplicitFunType>;
		MapType taskMap_;
	};

	// =========== CustomTaskExecutor_v2 ===========

	template<typename RetType, typename... Args>
	class CustomTaskExecutor_v2
	{
	public:
		template<typename T>
		static bool addTask(const std::string& name, T task)
		{
			CustomTaskExecutor_v2& inst = getInstance();
			//std::pair<MapType::iterator, bool> res = inst.taskMap_.insert(MapType::value_type{ name, std::move(task) }); //does not work on gcc
			auto res = inst.taskMap_.insert(typename MapType::value_type( name, std::move(task) ));
			if (!res.second)
				return false;

			return true;
		}

		static RetType runTask(const std::string& name, Args... args)
		{
			RetVal_v1<RetType> retVal;
			CustomTaskExecutor_v2& inst = getInstance();
			auto it = inst.taskMap_.find(name);
			if (it == inst.taskMap_.end())
				return retVal.get();

			return it->second(args...);
		}

		static RetType runTaskNoThrow(bool& success, const std::string& name, Args... args)
		{
			RetVal_v1<RetType> retVal;
			try
			{
				retVal.assign([=]() { return runTask(name, args...); });
				success = true;
			}
			catch (...)
			{
				success = false;
			}

			return retVal.get();
		}

	private:
		static CustomTaskExecutor_v2& getInstance()
		{
			static CustomTaskExecutor_v2 inst;
			return inst;
		}

		using ExplicitFunType = std::function<RetType(Args...)>;
		using MapType = std::unordered_map<std::string, ExplicitFunType>;
		MapType taskMap_;
	};

	// =========== CustomTaskExecutor_v3 ===========

	class CustomTaskExecutor_v3
	{
	private:
		static CustomTaskExecutor_v3 & getInstance()
		{
			static CustomTaskExecutor_v3 inst;
			return inst;
		}

		class TaskObject
		{
		public:
			virtual ~TaskObject() {} //Need implementation for pure virtual destructor
		};

		template<typename T>
		class TaskObjectImpl : public TaskObject
		{
		public:
			TaskObjectImpl(T&& task)
				: task_{ std::forward<T>(task) }
			{}
			~TaskObjectImpl() override = default;

			T getTask()
			{
				return task_;
			}

		private:
			T task_;
		};

		using MapType = std::unordered_map<std::string, std::shared_ptr<TaskObject>>;
		MapType taskMap_;

	public:
		template<typename RetType, typename ExplicitFunType>
		class Task
		{
		public:
			using TargetType = CustomTaskExecutor_v3::TaskObjectImpl<ExplicitFunType>;

			template<typename F>
			static bool addTask(const std::string& name, F task)
			{
				CustomTaskExecutor_v3& inst = CustomTaskExecutor_v3::getInstance();
				std::shared_ptr<TaskObject>& ptrTask = inst.taskMap_[name];

				if (ptrTask)
					return false;

				ptrTask = std::make_shared<TargetType>(std::move(task));
				return true;
			}

			template<typename... Args>
			static RetType runTask(const std::string& name, Args... args)
			{
				CustomTaskExecutor_v3& inst = CustomTaskExecutor_v3::getInstance();
				MapType::iterator it = inst.taskMap_.find(name);
				if (it == inst.taskMap_.end())
					throw std::runtime_error{ "Task of relevant type is not available" };

				auto ptrTaskImpl = std::dynamic_pointer_cast<TargetType>(it->second);
				if (!ptrTaskImpl)
					throw std::runtime_error{ "Either return type or the arguments to task are not maching" };

				return ptrTaskImpl->getTask()(args...);
			}

			template<typename... Args>
			static RetType runTaskNoThrow(bool& success, const std::string& name, Args... args)
			{
				RetVal_v1<RetType> retVal;
				try
				{
					retVal.assign([=]() { return Task::runTask(name, args...); });
					success = true;
				}
				catch (...)
				{
					success = false;
				}

				return retVal.get();
			}
		};
	};


	// =========== CustomTaskExecutor_v4 ===========

	class CustomTaskExecutor_v4
	{
	private:
		static CustomTaskExecutor_v4 & getInstance()
		{
			static CustomTaskExecutor_v4 inst;
			return inst;
		}

		class TaskObject
		{
		public:
			virtual ~TaskObject() {} //Need implementation for pure virtual destructor
		};

		template<typename T>
		class TaskObjectImpl : public TaskObject
		{
		public:
			TaskObjectImpl(T&& task)
				: task_{ std::forward<T>(task) }
			{}
			~TaskObjectImpl() override = default;

			T getTask()
			{
				return task_;
			}

		private:
			T task_;
		};

		using MapType = std::unordered_map<std::string, std::shared_ptr<TaskObject>>;
		MapType taskMap_;

	public:
		template<typename RetType, typename... Args>
		class Task
		{
		public:
			using FunType = std::function<RetType(Args...)>;
			using TargetType = CustomTaskExecutor_v4::TaskObjectImpl<FunType>;

			template<typename F>
			static bool addTask(const std::string& name, F task)
			{
				CustomTaskExecutor_v4& inst = CustomTaskExecutor_v4::getInstance();
				std::shared_ptr<TaskObject>& ptrTask = inst.taskMap_[name];

				if (ptrTask)
					return false;

				ptrTask = std::make_shared<TargetType>(std::move(task));
				return true;
			}

			static RetType runTask(const std::string& name, Args... args)
			{
				CustomTaskExecutor_v4& inst = CustomTaskExecutor_v4::getInstance();
				MapType::iterator it = inst.taskMap_.find(name);
				if (it == inst.taskMap_.end())
					throw std::runtime_error{ "Task of relevant type is not available" };

				auto ptrTaskImpl = std::dynamic_pointer_cast<TargetType>(it->second);
				if (!ptrTaskImpl)
					throw std::runtime_error{ "Either return type or the arguments to task are not maching" };

				return ptrTaskImpl->getTask()(args...);
			}

			static RetType runTaskNoThrow(bool& success, const std::string& name, Args... args)
			{
				RetVal_v1<RetType> retVal;
				try
				{
					retVal.assign([=]() { return Task::runTask(name, args...); });
					success = true;
				}
				catch (...)
				{
					success = false;
				}

				return retVal.get();
			}
		};
	
	};

}

