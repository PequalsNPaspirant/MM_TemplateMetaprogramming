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

	template<typename RetType>
	class RetVal_v1
	{
	public:
		RetVal_v1() {}
		RetVal_v1(const RetType& val)
			: val_{ val }
		{}

		RetVal_v1(const RetVal_v1&) = default;
		RetVal_v1(RetVal_v1&&) = default;
		RetVal_v1& operator=(const RetVal_v1&) = delete;
		RetVal_v1& operator=(RetVal_v1&&) = delete;

		RetType get()
		{
			return val_;
		}

		bool operator==(const RetVal_v1<RetType>& rhs)
		{
			return val_ == rhs.val_;
		}

		bool operator==(const RetType& rhsVal)
		{
			return val_ == rhsVal;
		}

		template<typename U, std::enable_if_t<!std::is_same_v<RetType, U>, void>* dummy = nullptr>
		//bool operator==(const RetVal_v1<U>& rhs)
		bool operator==(const U& rhs) //Ignore all other types than RetType and RetVal_v1<RetType>
		{
			return false;
		}

		template<typename F>
		void assign(F fun)
		{
			val_ = fun();
		}

		template<typename F, typename... Args>
		void assign(F fun, const std::string& name, Args... args)
		{
			val_ = fun(name, std::forward<Args>(args)...);
		}

		std::string toString()
		{
			stringstream ss;
			ss << val_;
			return ss.str();
		}

	private:
		RetType val_;
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

		template<typename F, typename... Args>
		void assign(F fun, const std::string& name, Args... args)
		{
			fun(name, std::forward<Args>(args)...);
		}

		std::string toString()
		{
			return "";
		}
	};


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
			virtual ~TaskObject() {} //Need implementation for pure virtual destructor
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

			//static bool runTaskNoThrow(const std::string& name, const Args&... args) //This version is called for return type: void
			//{
			//	try { TaskBase<RetType, Args...>::getTask(name)(args...); }
			//	catch (...) { return false; }

			//	return true;
			//}

			static bool runTaskNoThrow(const std::string& name, Args... args) //This version is called for return type: void
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

			static bool runTaskNoThrow(const std::string& name) //This version is called for return type: void
			{
				try { TaskBase<RetType>::getTask(name)(); }
				catch (...) { return false; }

				return true;
			}

		};
	
	};

}

