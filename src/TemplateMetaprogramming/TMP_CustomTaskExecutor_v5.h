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

	// =========== CustomTaskExecutor_v5 ===========

	class CustomTaskExecutor_v5
	{
	private:
		static CustomTaskExecutor_v5 & getInstance()
		{
			static CustomTaskExecutor_v5 inst;
			return inst;
		}

		class TaskObject
		{
		public:
			virtual ~TaskObject() = default;
		};

		template<typename TaskType>
		class TaskObjectImpl : public TaskObject
		{
		public:
			TaskObjectImpl(TaskType task)
				: task_{ task }
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

		template<typename RetType, typename... Args>
		class TaskBase
		{
		public:
			using FunType = std::function<RetType(Args...)>;
			using TargetType = CustomTaskExecutor_v5::TaskObjectImpl<FunType>;

			template<typename F>
			static bool addTask(const std::string& name, F task)
			{
				CustomTaskExecutor_v5& inst = CustomTaskExecutor_v5::getInstance();
				std::shared_ptr<TaskObject>& ptrTask = inst.taskMap_[name];

				if (ptrTask)
					return false;

				ptrTask = std::make_shared<TargetType>(task);
				return true;
			}

			static FunType getTask(const std::string& name)
			{
				CustomTaskExecutor_v5& inst = CustomTaskExecutor_v5::getInstance();
				MapType::iterator it = inst.taskMap_.find(name);
				if (it == inst.taskMap_.end())
					throw std::runtime_error{ "Task '" + name + "' is not added" };

				auto ptrTaskImpl = std::dynamic_pointer_cast<TargetType>(it->second);
				if (!ptrTaskImpl)
					throw std::runtime_error{ "Either return type or the arguments to task are not maching" };

				return ptrTaskImpl->getTask();
			}

		};

	public:
		template<typename RetType, typename... Args>
		class Task : public TaskBase<RetType, Args...>
		{
		public:
			//static RetType runTask(const std::string& name, const Args&... args)
			//{
			//	return TaskBase<RetType, Args...>::getTask(name)(args...);
			//}

			static RetType runTask(const std::string& name, Args... args)
			{
				return TaskBase<RetType, Args...>::getTask(name)(forward<Args>(args)...);
			}

			//template<typename LocalRetType>
			//static bool runTaskNoThrow(const std::string& name, LocalRetType& retVal, const Args&... args)
			//{
			//	try { retVal = TaskBase<RetType, Args...>::getTask(name)(args...); }
			//	catch (...) { return false; }

			//	return true;
			//}

			template<typename LocalRetType>
			static bool runTaskNoThrow(const std::string& name, LocalRetType& retVal, Args... args)
			{
				try { retVal = TaskBase<RetType, Args...>::getTask(name)(forward<Args>(args)...); }
				catch (...) { return false; }

				return true;
			}

			//static bool runTaskNoThrow(const std::string& name, const Args&... args) //This version is called for return type 'void' or if someone is not interested in the return value
			//{
			//	try { TaskBase<RetType, Args...>::getTask(name)(args...); }
			//	catch (...) { return false; }

			//	return true;
			//}

			static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type 'void' or if someone is not interested in the return value
			{
				try { TaskBase<RetType, Args...>::getTask(name)(forward<Args>(args)...); }
				catch (...) { return false; }

				return true;
			}
		};

		//Specialization for zero arguments
		template<typename RetType>
		class Task<RetType> : public TaskBase<RetType>
		{
		public:
			static RetType runTask(const std::string& name)
			{
				return TaskBase<RetType>::getTask(name)();
			}

			template<typename LocalRetType>
			static bool runTaskNoThrow(const std::string& name, LocalRetType& retVal)
			{
				try { retVal = TaskBase<RetType>::getTask(name)(); }
				catch (...) { return false; }

				return true;
			}

			static bool runTaskNoThrow(const std::string& name) //This version is called for return type 'void' or if someone is not interested in the return value
			{
				try { TaskBase<RetType>::getTask(name)(); }
				catch (...) { return false; }

				return true;
			}

		};
	
	};

}

