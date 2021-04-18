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
			CustomTaskExecutor_v1& inst = getInstance();
			auto it = inst.taskMap_.find(name);
			if (it == inst.taskMap_.end())
				throw std::runtime_error{ "Task '" + name + "' is not added" };

			return it->second(args...);
		}

		template<typename RetTypeExpected, typename... Args>
		static bool runTaskNoThrow(RetTypeExpected& retValEx, const std::string& name, Args... args)
		{
			try
			{
				//RetVal_v1<RetType> retVal;
				//retVal.assign([=]() { return runTask(name, args...); });
				//retValEx = std::move(retVal.get());
				retValEx = runTask(name, args...);
			}
			catch (...)
			{
				return false;
			}

			return true;
		}

		template<typename... Args>
		static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type: void
		{
			try
			{
				runTask(name, args...);
			}
			catch (...)
			{
				return false;
			}

			return true;
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
			CustomTaskExecutor_v2& inst = getInstance();
			auto it = inst.taskMap_.find(name);
			if (it == inst.taskMap_.end())
				throw std::runtime_error{ "Task '" + name + "' is not added" };

			return it->second(args...);
		}

		template<typename RetTypeExpected>
		static bool runTaskNoThrow(RetTypeExpected& retValEx, const std::string& name, Args... args)
		{
			try
			{
				//RetVal_v1<RetType> retVal;
				//retVal.assign([=]() { return runTask(name, args...); });
				//retValEx = std::move(retVal.get());
				retValEx = runTask(name, args...);
			}
			catch (...)
			{
				return false;
			}

			return true;
		}

		static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type: void
		{
			try
			{
				runTask(name, args...);
			}
			catch (...)
			{
				return false;
			}

			return true;
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

		template<typename TaskType>
		class TaskObjectImpl : public TaskObject
		{
		public:
			TaskObjectImpl(TaskType&& task)
				: task_{ std::forward<TaskType>(task) }
			{}
			~TaskObjectImpl() override = default;

			TaskType getTask()
			{
				return task_;
			}

		private:
			TaskType task_;
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
				return getTask(name)(args...);
			}

			template<typename RetTypeExpected, typename... Args>
			static bool runTaskNoThrow(RetTypeExpected& retValEx, const std::string& name, Args... args)
			{
				try
				{
					//RetVal_v1<RetType> retVal;
					//retVal.assign([=]() { return Task::runTask(name, args...); });
					//retValEx = std::move(retVal.get());
					retValEx = runTask(name, args...);
				}
				catch (...)
				{
					return false;
				}

				return true;
			}

			template<typename... Args>
			static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type: void
			{
				try
				{
					Task::runTask(name, args...);
				}
				catch (...)
				{
					return false;
				}

				return true;
			}

		private:
			static ExplicitFunType getTask(const std::string& name)
			{
				CustomTaskExecutor_v3& inst = CustomTaskExecutor_v3::getInstance();
				MapType::iterator it = inst.taskMap_.find(name);
				if (it == inst.taskMap_.end())
					throw std::runtime_error{ "Task '" + name + "' is not added" };

				auto ptrTaskImpl = std::dynamic_pointer_cast<TargetType>(it->second);
				if (!ptrTaskImpl)
					throw std::runtime_error{ "Either return type or the arguments to task are not maching" };

				return ptrTaskImpl->getTask();
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

		template<typename TaskType>
		class TaskObjectImpl : public TaskObject
		{
		public:
			TaskObjectImpl(TaskType&& task)
				: task_{ std::forward<TaskType>(task) }
			{}
			~TaskObjectImpl() override = default;

			TaskType getTask()
			{
				return task_;
			}

		private:
			TaskType task_;
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
				return getTask(name)(args...);
			}

			template<typename RetTypeExpected>
			static bool runTaskNoThrow(RetTypeExpected& retValEx, const std::string& name, Args... args)
			{
				try
				{
					//RetVal_v1<RetType> retVal;
					//retVal.assign([=]() { return Task::runTask(name, args...); });
					//retValEx = std::move(retVal.get());
					retValEx = runTask(name, args...);
				}
				catch (...)
				{
					return false;
				}

				return true;
			}

			static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type: void
			{
				try
				{
					Task::runTask(name, args...);
				}
				catch (...)
				{
					return false;
				}

				return true;
			}

		private:
			static FunType getTask(const std::string& name)
			{
				CustomTaskExecutor_v4& inst = CustomTaskExecutor_v4::getInstance();
				MapType::iterator it = inst.taskMap_.find(name);
				if (it == inst.taskMap_.end())
					throw std::runtime_error{ "Task '" + name + "' is not added" };

				auto ptrTaskImpl = std::dynamic_pointer_cast<TargetType>(it->second);
				if (!ptrTaskImpl)
					throw std::runtime_error{ "Either return type or the arguments to task are not maching" };

				return ptrTaskImpl->getTask();
			}
		};
	
	};

}

