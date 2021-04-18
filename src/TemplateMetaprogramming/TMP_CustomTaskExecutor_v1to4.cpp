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
#include "TMP_CustomTaskExecutor_v1to4.h"

namespace mm {

	// =========== Test Functions ===========

	//c-style functions
	void cStyleFunVoidVoid()
	{
		std::cout << "cStyleFunVoidVoid()";
	}
	void cStyleFunVoidInt(int n)
	{
		std::cout << "cStyleFunVoidInt(int n) Args: n: " << n;
	}
	std::string cStyleFunStringDouble(double d)
	{
		std::cout << "std::string cStyleFunStringDouble(double d) Args: d: " << d;
		return std::string{"Args: "} + std::to_string(d);
	}
	std::string cStyleFunStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll)
	{
		std::cout << "std::string cStyleFunStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll) Args: s: " << s << " d: " << d << " ll: " << ll;
		return std::string{ "Args: " } + s + "," + std::to_string(d) + "," + std::to_string(ll);
	}

	//functors
	class Functor
	{
	public:
		void operator()() const
		{
			std::cout << "operator()()";
		}
		void operator()(int n) const
		{
			std::cout << "operator()(int n) Args: n: " << n;
		}
		std::string operator()(double d) const
		{
			std::cout << "std::string operator()(double d) Args: d: " << d;
			return std::string{ "Args: " } +std::to_string(d);
		}
		std::string operator()(const std::string& s, double d, long long ll) const
		{
			std::cout << "std::string operator()(const std::string& s, double d, long long ll) Args: s: " << s << " d: " << d << " ll: " << ll;
			return std::string{ "Args: " } +s + "," + std::to_string(d) + "," + std::to_string(ll);
		}
	};

	//lambdas
	auto lambdaVoidVoid = []()
	{
		std::cout << "lambdaVoidVoid()";
	};
	auto lambdaVoidInt = [](int n)
	{
		std::cout << "lambdaVoidInt(int n) Args: n: " << n;
	};
	auto lambdaStringDouble = [](double d) -> std::string
	{
		std::cout << "std::string lambdaStringDouble(double d) Args: d: " << d;
		return std::string{ "Args: " } +std::to_string(d);
	};
	auto lambdaStringConstRefStringDoubleLonglong = [](const std::string& s, double d, long long ll) -> std::string
	{
		std::cout << "std::string lambdaStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll) Args: s: " << s << " d: " << d << " ll: " << ll;
		return std::string{ "Args: " } +s + "," + std::to_string(d) + "," + std::to_string(ll);
	};

	//std::functions
	std::function<void(void)> stdFunVoidVoid = []()
	{
		std::cout << "lambdaVoidVoid()";
	};
	std::function<void(int)> stdFunVoidInt = [](int n)
	{
		std::cout << "lambdaVoidInt(int n) Args: n: " << n;
	};
	std::function<std::string(double)> stdFunStringDouble = [](double d) -> std::string
	{
		std::cout << "std::string lambdaStringDouble(double d) Args: d: " << d;
		return std::string{ "Args: " } +std::to_string(d);
	};
	std::function<std::string(const std::string&, double, long long)> stdFunStringConstRefStringDoubleLonglong = [](const std::string& s, double d, long long ll) -> std::string
	{
		std::cout << "std::string lambdaStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll) Args: s: " << s << " d: " << d << " ll: " << ll;
		return std::string{ "Args: " } +s + "," + std::to_string(d) + "," + std::to_string(ll);
	};


	// =========== Helper Classes / Functions ===========

	template<typename RetType>
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

		std::string toString()
		{
			return "";
		}
	};
	
	template<template<typename... Args> class CustomTaskExecutor, typename RetType, typename... ExplicitFunTypeOrArgs>
	struct TestFunctionStructHelper
	{
		template<typename... Args>
		static RetType testFunctionHelper(bool& success, const std::string& funName, Args... args)
		{
			RetType retVal;
			success = CustomTaskExecutor<RetType, ExplicitFunTypeOrArgs...>::runTaskNoThrow(retVal, funName, args...);
			return retVal;
		}
	};

	//Partial specialization for RetType = void
	template<template<typename... Args> class CustomTaskExecutor, typename... ExplicitFunTypeOrArgs>
	struct TestFunctionStructHelper<CustomTaskExecutor, void, ExplicitFunTypeOrArgs...>
	{
		template<typename... Args>
		static void testFunctionHelper(bool& success, const std::string& funName, Args... args)
		{
			success = CustomTaskExecutor<void, ExplicitFunTypeOrArgs...>::runTaskNoThrow(funName, args...);
			return;
		}
	};

	template<template<typename... Args> class CustomTaskExecutor, typename RetType, typename... ExplicitFunTypeOrArgs>
	struct TestFunctionStruct
	{
		template<typename ActualFunType, typename... Args>
		static void testFunction(const std::string& funName, ActualFunType task, Args... args)
		{
			bool success1 = false;
			bool success2 = false;
			RetVal_v1<RetType> retVal1;
			RetVal_v1<RetType> retVal2;

			std::cout << "\n\n" << funName << ":";
			std::cout << "\n" << "addTask: "; success1 = CustomTaskExecutor<RetType, ExplicitFunTypeOrArgs...>::addTask(funName, task); std::cout << " success1: " << success1;
			std::cout << "\n" << "addTask: "; success2 = CustomTaskExecutor<RetType, ExplicitFunTypeOrArgs...>::addTask(funName, task); std::cout << " success2: " << success2;
			auto lambda1 = [=]() { return CustomTaskExecutor<RetType, ExplicitFunTypeOrArgs...>::runTask(funName, args...); };
			std::cout << "\n" << "runTask: "; retVal1.assign(lambda1); std::cout << " retVal1: '" << retVal1.toString() << "'";
			bool success3 = false;
			auto lambda2 = [=, &success3]() {
				//return CustomTaskExecutor<RetType, ExplicitFunTypeOrArgs...>::runTaskNoThrow(success3, funName, args...);
				return TestFunctionStructHelper<CustomTaskExecutor, RetType, ExplicitFunTypeOrArgs...>::testFunctionHelper(success3, funName, args...);
			};
			std::cout << "\n" << "runTaskNoThrow: "; retVal2.assign(lambda2); std::cout << " retVal2: '" << retVal2.toString() << "' success3: " << success3;

			RetVal_v1<RetType> expectedOutput;
			std::cout << "\n" << "run actual task to get expected output: "; expectedOutput.assign([=]() { return task(args...); });

			cout << "\n\n";
			MM_EXPECT_TRUE(success1 == true);
			MM_EXPECT_TRUE(success2 == false);
			MM_EXPECT_TRUE(retVal1 == expectedOutput, retVal1.toString(), expectedOutput.toString());
			MM_EXPECT_TRUE(success3 == true);
			MM_EXPECT_TRUE(retVal2 == expectedOutput, retVal2.toString(), expectedOutput.toString());

		}
	};


	template<template<typename... Args> class CustomTaskExecutor>
	void testCustomTaskExecutor_v1v3()
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

		using FunType1 = std::function<void(void)>;
		using FunType2 = decltype(cStyleFunVoidVoid);
		using FunType3 = std::function<void(int)>;
		using FunType4 = decltype(cStyleFunVoidInt);
		using FunType5 = std::function<std::string(double)>;
		using FunType6 = decltype(cStyleFunStringDouble);
		using FunType7 = std::function<std::string(const std::string&, double, long long)>;
		using FunType8 = decltype(cStyleFunStringConstRefStringDoubleLonglong);

//#if defined(_MSC_VER)
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType1>::testFunction(funName, cStyleFunVoidVoid);
		//incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType2>::testFunction(funName, cStyleFunVoidVoid); //Error while inserting into unordered_map: No constructor could take the source type, or constructor overload resolution was ambiguous
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType3>::testFunction(funName, cStyleFunVoidInt, intIn);
		//incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType4>::testFunction(funName, cStyleFunVoidInt, intIn);
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType5>::testFunction(funName, cStyleFunStringDouble, dIn);
		//incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType6>::testFunction(funName, cStyleFunStringDouble, dIn);
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType7>::testFunction(funName, cStyleFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
		//incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType8>::testFunction(funName, cStyleFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
//#endif

		incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType1>::testFunction(funName, Functor{});
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType3>::testFunction(funName, Functor{}, intIn);
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType5>::testFunction(funName, Functor{}, dIn);
		incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType7>::testFunction(funName, Functor{}, strIn, dIn, llIn);

		{
			using FunType2 = decltype(lambdaVoidVoid);
			using FunType4 = decltype(lambdaVoidInt);
			using FunType6 = decltype(lambdaStringDouble);
			using FunType8 = decltype(lambdaStringConstRefStringDoubleLonglong);

			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType1>::testFunction(funName, lambdaVoidVoid);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType2>::testFunction(funName, lambdaVoidVoid);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType3>::testFunction(funName, lambdaVoidInt, intIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType4>::testFunction(funName, lambdaVoidInt, intIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType5>::testFunction(funName, lambdaStringDouble, dIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType6>::testFunction(funName, lambdaStringDouble, dIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType7>::testFunction(funName, lambdaStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType8>::testFunction(funName, lambdaStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
		}

		{
			using FunType2 = decltype(stdFunVoidVoid);
			using FunType4 = decltype(stdFunVoidInt);
			using FunType6 = decltype(stdFunStringDouble);
			using FunType8 = decltype(stdFunStringConstRefStringDoubleLonglong);

			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType1>::testFunction(funName, stdFunVoidVoid);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType2>::testFunction(funName, stdFunVoidVoid);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType3>::testFunction(funName, stdFunVoidInt, intIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, void, FunType4>::testFunction(funName, stdFunVoidInt, intIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType5>::testFunction(funName, stdFunStringDouble, dIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType6>::testFunction(funName, stdFunStringDouble, dIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType7>::testFunction(funName, stdFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
			incrementInput(); TestFunctionStruct<CustomTaskExecutor, std::string, FunType8>::testFunction(funName, stdFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
		}
	}

	template<template<typename... Args> class CustomTaskExecutor>
	void testCustomTaskExecutor_v2v4()
	{
		std::cout << std::boolalpha;
		std::string funName;
		int funIndex = 0;
		int intIn = 0;
		long long llIn = 0;
		double dIn = 1.000;
		std::string strIn{ "aa" };

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

		using TestFunctionStruct1 = TestFunctionStruct<CustomTaskExecutor, void>;
		using TestFunctionStruct2 = TestFunctionStruct<CustomTaskExecutor, void, int>;
		using TestFunctionStruct3 = TestFunctionStruct<CustomTaskExecutor, std::string, double>;
		using TestFunctionStruct4 = TestFunctionStruct<CustomTaskExecutor, std::string, const std::string&, double, long long>;

//#if defined(_MSC_VER)
		incrementInput(); TestFunctionStruct1::testFunction(funName, cStyleFunVoidVoid);
		incrementInput(); TestFunctionStruct2::testFunction(funName, cStyleFunVoidInt, intIn);
		incrementInput(); TestFunctionStruct3::testFunction(funName, cStyleFunStringDouble, dIn);
		incrementInput(); TestFunctionStruct4::testFunction(funName, cStyleFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
//#endif

		incrementInput(); TestFunctionStruct1::testFunction(funName, Functor{});
		incrementInput(); TestFunctionStruct2::testFunction(funName, Functor{}, intIn);
		incrementInput(); TestFunctionStruct3::testFunction(funName, Functor{}, dIn);
		incrementInput(); TestFunctionStruct4::testFunction(funName, Functor{}, strIn, dIn, llIn);

		incrementInput(); TestFunctionStruct1::testFunction(funName, lambdaVoidVoid);
		incrementInput(); TestFunctionStruct2::testFunction(funName, lambdaVoidInt, intIn);
		incrementInput(); TestFunctionStruct3::testFunction(funName, lambdaStringDouble, dIn);
		incrementInput(); TestFunctionStruct4::testFunction(funName, lambdaStringConstRefStringDoubleLonglong, strIn, dIn, llIn);

		incrementInput(); TestFunctionStruct1::testFunction(funName, stdFunVoidVoid);
		incrementInput(); TestFunctionStruct2::testFunction(funName, stdFunVoidInt, intIn);
		incrementInput(); TestFunctionStruct3::testFunction(funName, stdFunStringDouble, dIn);
		incrementInput(); TestFunctionStruct4::testFunction(funName, stdFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
	}

	void testAllCustomTaskExecutorVersions()
	{
		testCustomTaskExecutor_v1v3<CustomTaskExecutor_v1>();
		testCustomTaskExecutor_v2v4<CustomTaskExecutor_v2>();
		testCustomTaskExecutor_v1v3<CustomTaskExecutor_v3::Task>();
		testCustomTaskExecutor_v2v4<CustomTaskExecutor_v4::Task>();
	}

	MM_DECLARE_FLAG(TemplateMetaProgrammingCustomTaskExecutor_v1);

	MM_UNIT_TEST(TemplateMetaProgrammingCustomTaskExecutorTest_v1, TemplateMetaProgrammingCustomTaskExecutor_v1)
	{
		testAllCustomTaskExecutorVersions();
	}
}

