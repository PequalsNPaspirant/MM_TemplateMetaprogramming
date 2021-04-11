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
		bool operator==(const U& rhs)
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
		void assign(F fun)
		{
			fun();
		}

		std::string toString()
		{
			return "";
		}
	};

	template<typename RetType, typename FunType>
	class CustomTaskExecutor_v1
	{
	public:
		static bool addTask(const std::string& name, FunType fun)
		{
			CustomTaskExecutor_v1& inst = getInstance();
			auto it = inst.taskMap_.find(name);
			if (it != inst.taskMap_.end())
				return false;

			inst.taskMap_[name] = fun;
			return true;
		}

		template<typename... Args>
		static RetType runTask(const std::string& name, Args... args)
		{
			RetVal_v1<RetType> ret;
			CustomTaskExecutor_v1& inst = getInstance();
			auto it = inst.taskMap_.find(name);
			if (it == inst.taskMap_.end())
				return ret.get();

			return it->second(args...);
		}

	private:
		static CustomTaskExecutor_v1& getInstance()
		{
			static CustomTaskExecutor_v1 inst;
			return inst;
		}
		std::unordered_map<std::string, FunType> taskMap_;
	};

	void cStyleFunVoidVoid()
	{
		std::cout << "cStyleFunVoidVoid()";
	}
	void cStyleFunVoidInt(int n)
	{
		std::cout << "cStyleFunVoidInt() Args: n: " << n;
	}
	std::string cStyleFunStringDouble(double d)
	{
		std::cout << "cStyleFunStringDouble() Args: d: " << d;
		return std::string{"Args: "} + std::to_string(d);
	}
	std::string cStyleFunStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll)
	{
		std::cout << "cStyleFunStringConstRefStringDoubleLonglong() Args: s: " << s << " d: " << d << " ll: " << ll;
		return std::string{ "Args: " } + s + "," + std::to_string(d) + "," + std::to_string(ll);
	}

	template<typename RetType, typename ExplicitFunType>
	struct TestFunctionStruct
	{
		template<typename ActualFunType, typename... Args>
		static void testFunction(const std::string& funName, ActualFunType fun, Args... args)
		{
			bool success1 = false;
			bool success2 = false;
			RetVal_v1<RetType> retVal;

			std::cout << "\n\n" << funName << ":";
			std::cout << "\n" << "addTask: "; success1 = CustomTaskExecutor_v1<RetType, ExplicitFunType>::addTask(funName, fun); std::cout << " success1: " << success1;
			std::cout << "\n" << "addTask: "; success2 = CustomTaskExecutor_v1<RetType, ExplicitFunType>::addTask(funName, fun); std::cout << " success2: " << success2;
			auto lambda = [=]() { return CustomTaskExecutor_v1<RetType, ExplicitFunType>::runTask(funName, args...); };
			std::cout << "\n" << "runTask: "; retVal.assign(lambda); std::cout << " retVal: '" << retVal.toString() << "'";

			RetVal_v1<RetType> expectedOutput;
			std::cout << "\n" << "run actual fun to get expected output: "; expectedOutput.assign([=]() { return fun(args...); });

			cout << "\n\n";
			MM_EXPECT_TRUE(success1 == true);
			MM_EXPECT_TRUE(success2 == false);
			MM_EXPECT_TRUE(retVal == expectedOutput, retVal.toString(), expectedOutput.toString());
			
		}
	};

	void testCustomTaskExecutor()
	{
		std::cout << std::boolalpha;
		std::string funName;
		int funIndex = 0;
		int intIn = 0;
		long long llIn = 0;
		double dIn = 1.000;		
		std::string strIn{"aa"};

		auto incrementInput = [&]() {
			intIn += 1;
			funName = "fun_" + std::to_string(intIn);
			llIn += 10;
			dIn += 0.001;
			for (int i = static_cast<int>(strIn.length() - 1); i >= 0; --i)
			{
				if (strIn[i] < 'z')
				{
					++strIn[i];
					break;
				}
				else
					strIn[i] = 'a';
			}
		};
		//bool success = false;
		//std::string retVal{""};

		//std::cout << "\n" << "fun1:";
		using FunType1 = std::function<void(void)>;
		//std::cout << "\n" << "addTask: "; success = CustomTaskExecutor_v1<void, FunType1>::addTask("fun1", cStyleFunVoidVoid);
		//std::cout << "\n" << "runTask: "; CustomTaskExecutor_v1<void, FunType1>::runTask("fun1");
		//std::cout << "\n" << "success: " << success << " retVal: " << retVal;
		incrementInput();
		TestFunctionStruct<void, FunType1>::testFunction(funName, cStyleFunVoidVoid);

		//std::cout << "\n" << "fun2:";
		using FunType2 = std::function<void(int)>;
		//std::cout << "\n" << "addTask: "; success = CustomTaskExecutor_v1<void, FunType2>::addTask("fun2", cStyleFunVoidInt);
		//std::cout << "\n" << "runTask: "; CustomTaskExecutor_v1<void, FunType2>::runTask("fun2", 3);
		//std::cout << "\n" << "success: " << success << " retVal: " << retVal;
		incrementInput();
		TestFunctionStruct<void, FunType2>::testFunction(funName, cStyleFunVoidInt, intIn);

		//std::cout << "\n" << "fun3:";
		using FunType3 = std::function<std::string(double)>;
		//std::cout << "\n" << "addTask: "; success = CustomTaskExecutor_v1<std::string, FunType3>::addTask("fun3", cStyleFunStringDouble);
		//std::cout << "\n" << "runTask: "; retVal = CustomTaskExecutor_v1<std::string, FunType3>::runTask("fun3", 11.11);
		//std::cout << "\n" << "success: " << success << " retVal: " << retVal;
		incrementInput();
		TestFunctionStruct<std::string, FunType3>::testFunction(funName, cStyleFunStringDouble, dIn);

		//std::cout << "\n" << "fun4:";
		using FunType4 = std::function<std::string(const std::string&, double, long long)>;
		//std::cout << "\n" << "addTask: "; success = CustomTaskExecutor_v1<std::string, FunType4>::addTask("fun4", cStyleFunStringStringDoubleLonglong);
		//std::cout << "\n" << "runTask: "; retVal = CustomTaskExecutor_v1<std::string, FunType4>::runTask("fun4", "aaa", 22.22, 2222);
		//std::cout << "\n" << "success: " << success << " retVal: " << retVal;
		incrementInput();
		TestFunctionStruct<std::string, FunType4>::testFunction(funName, cStyleFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);

	}

	MM_DECLARE_FLAG(TemplateMetaProgrammingCustomTaskExecutor_v1);

	MM_UNIT_TEST(TemplateMetaProgrammingCustomTaskExecutorTest_v1, TemplateMetaProgrammingCustomTaskExecutor_v1)
	{
		testCustomTaskExecutor();
	}
}

