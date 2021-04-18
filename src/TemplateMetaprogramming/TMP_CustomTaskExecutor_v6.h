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

	namespace customTaskExecutor_v6 {

		// =========== CustomTaskExecutor ===========

		class CustomTaskExecutor
		{
		private:
			static CustomTaskExecutor & getInstance()
			{
				static CustomTaskExecutor inst;
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

		public:
			template<typename RetType, typename... Args>
			class Task
			{
			public:
				using FunType = std::function<RetType(Args...)>;
				using TargetType = CustomTaskExecutor::TaskObjectImpl<FunType>;

				template<typename F>
				static bool addTask(const std::string& name, F task)
				{
					CustomTaskExecutor& inst = CustomTaskExecutor::getInstance();
					std::shared_ptr<TaskObject>& ptrTask = inst.taskMap_[name];

					if (ptrTask)
						return false;

					ptrTask = std::make_shared<TargetType>(task);
					return true;
				}

				static RetType runTask(const std::string& name, Args... args)
				{
					return getTask(name)(forward<Args>(args)...);
				}

				template<typename LocalRetType>
				static bool runTaskNoThrow(const std::string& name, LocalRetType& retVal, Args... args)
				{
					try { retVal = getTask(name)(forward<Args>(args)...); }
					catch (...) { return false; }

					return true;
				}

				static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type 'void' or if someone is not interested in the return value
				{
					try { getTask(name)(forward<Args>(args)...); }
					catch (...) { return false; }

					return true;
				}

			private:
				static FunType getTask(const std::string& name)
				{
					CustomTaskExecutor& inst = CustomTaskExecutor::getInstance();
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

}

