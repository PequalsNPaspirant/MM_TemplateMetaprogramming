//Goal
//All miscellaneous problems

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
using namespace std;

namespace mm {

	template <typename T>
	void FunTakesValueOrPointer(T input)
	{
		cout << "\nThe type: " << typeid(T).name();
	}

	template <typename T>
	void FunAlwaysTakesPointer(T* input)
	{
		cout << "\nThe pointer of type: " << typeid(T).name();
	}

	void testArgumentTypesToTemplate()
	{
		int n = 10;
		double d = 10.12;
		int* pn = nullptr;
		double* pd = &d;

		FunTakesValueOrPointer(n);
		FunTakesValueOrPointer(d);
		FunTakesValueOrPointer(pn);
		FunTakesValueOrPointer(pd);

		//	FunAlwaysTakesPointer(n); //error C2784: 'void FunAlwaysTakesPointer(T *)': could not deduce template argument for 'T *' from 'int'
		//	FunAlwaysTakesPointer(d); //error C2784: 'void FunAlwaysTakesPointer(T *)': could not deduce template argument for 'T *' from 'double'
		FunAlwaysTakesPointer(pn);
		FunAlwaysTakesPointer(pd);
	}

	//#include <cxxabi.h>

	//template<typename T>
	//std::string type_name()
	//{
	//	int status;
	//	std::string tname = typeid(T).name();
	//	char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status); //Available only in GCC
	//	if (status == 0) {
	//		tname = demangled_name;
	//		std::free(demangled_name);
	//	}
	//	return tname;
	//}
	//
	//class MyExampleClass
	//{
	//
	//};
	//
	//void testGetTypeName()
	//{
	//	cout << "\nType name is: " << type_name<int>;
	//	cout << "\nType name is: " << type_name<double*>;
	//	cout << "\nType name is: " << type_name<MyExampleClass>;
	//	cout << "\nType name is: " << type_name<MyExampleClass**>;
	//}

	void Templates()
	{
		//testArgumentTypesToTemplate();
		//testGetTypeName();
	}

	template<typename RetType, typename... Args>
	struct convertToFun
	{
		template<typename T>
		std::function<RetType(Args...)> operator()(T task)
		{
			std::function<RetType(Args...)> fun = [&](Args... args) { return task(std::forward<Args>(args)...) };
			return fun;
		}
	};
	

	class CustomTaskExecutor
	{
	public:
		static const CustomTaskExecutor& getInstance()
		{
			static CustomTaskExecutor inst;
			return inst;
		}

		enum class TaskName
		{
			dosomething
		};

		template<typename RetType, typename... Args>
		void addTask(TaskName name, std::function<RetType(Args...)> task) const
		{
			taskMap_[name] = std::make_shared<TaskImpl<RetType, Args...>>(task);
		}

		//template<typename T>
		//void addTask(TaskName name, T task) const
		//{
		//	using FunType = std::function<RetType(Args...)>;
		//	FunType fun = [](Args... args) {
		//		task(args...);
		//	};
		//	addTask(name, fun);
		//}

		template<typename RetType, typename... Args>
		RetType runTask(TaskName name, Args... args) const
		{
			auto it = taskMap_.find(name);
			if (it == taskMap_.end())
				return;

			using FunType = std::function<RetType(Args...)>;
			std::shared_ptr<FunType> fun = std::dynamic_pointer_cast<FunType>(it->second);
			if (!fun)
				throw std::runtime_error{ "wrong function argument or return type" };

			return it->second(args...);
		}

	private:

		class Task;
		using TaskRW = std::shared_ptr<Task>;
		class Task
		{
		public:
		};

		template<typename RetType, typename... Args>
		class TaskImpl : public Task
		{
		public:
			TaskImpl(std::function<RetType(Args...)> task)
				: task_{ task }
			{}

		private:
			std::function<RetType(Args...)> task_;
		};

		mutable std::unordered_map<TaskName, TaskRW> taskMap_;
	};

	int fun2(std::string& s)
	{
		return 0;
	}

	class fun3
	{
	public:
		char operator()(const long long& num) const
		{
			return 'a';
		}
	};

	void usage()
	{
		std::function<std::string(double)> fun1 = [](double flag) -> std::string {
			return std::string{};
		};
		CustomTaskExecutor::getInstance().addTask(CustomTaskExecutor::TaskName::dosomething, fun1);

		CustomTaskExecutor::getInstance().addTask(CustomTaskExecutor::TaskName::dosomething, convertToFun<int, std::string&>{}(fun2));

		CustomTaskExecutor::getInstance().addTask(CustomTaskExecutor::TaskName::dosomething, convertToFun<char, const long long&>{}(fun3{}));

		auto fun4 = [](bool flag) {
			return 2.3;
		};
		CustomTaskExecutor::getInstance().addTask(CustomTaskExecutor::TaskName::dosomething, convertToFun<double, bool>{}(fun4));
	}

}
