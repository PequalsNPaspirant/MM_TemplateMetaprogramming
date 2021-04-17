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
#include "TMP_CustomTaskExecutor_v5.h"

namespace mm {

	namespace customTaskExecutor_v5 {

		// =========== Test functions ===========

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
			return std::string{ "Args: " } + std::to_string(d);
		}
		std::string cStyleFunStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll)
		{
			std::cout << "std::string cStyleFunStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll) Args: s: " << s << " d: " << d << " ll: " << ll;
			return std::string{ "Args: " } + s + "," + std::to_string(d) + "," + std::to_string(ll);
		}
		std::string cStyleFunStrStrConstStrRefStrConstRefStrRValRefStr(
			std::string s, const std::string cs, std::string& rs, const std::string& crs, std::string&& rvrs)
		{
			std::cout << "std::string cStyleFunStrStrConstStrRefStrConstRefStrRValRefStr("
				<< "std::string s, const std::string cs, std::string& rs, const std::string& crs, std::string&& rvrs) Args:"
				<< " s: " << s
				<< " cs: " << cs
				<< " rs: " << rs
				<< " crs: " << crs
				<< " rvrs: " << rvrs;
			s += "-changed";
			//cs += "-changed";
			rs += "-changed";
			//crs += "-changed";
			std::string sCopy = std::move(rvrs);
			return std::string{ "Args: " } + s + "," + cs + "," + rs + "," + crs + "," + rvrs;
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




		template<template<typename... Args> class CustomTaskExecutor, typename RetType, typename... Args>
		struct TestFunctionStructHelper
		{
			static RetVal_v1<RetType> runTaskHelper(const std::string& funName, Args... args)
			{
				RetType retVal = CustomTaskExecutor<RetType, Args...>::runTask(funName, std::forward<Args>(args)...);
				return RetVal_v1<RetType>{ retVal };
			}

			static RetVal_v1<RetType> runTaskNoThrowHelper(bool& success, const std::string& funName, Args... args)
			{
				RetType retVal;
				success = CustomTaskExecutor<RetType, Args...>::runTaskNoThrow(funName, retVal, std::forward<Args>(args)...);
				return RetVal_v1<RetType>{ retVal };
			}

			template<typename F>
			static RetVal_v1<RetType> directFunCallHelper(F task, Args... args)
			{
				RetType retVal = task(std::forward<Args>(args)...);
				return RetVal_v1<RetType>{ retVal };
			}
		};

		//Partial specialization for RetType = void
		template<template<typename... Args> class CustomTaskExecutor, typename... Args>
		struct TestFunctionStructHelper<CustomTaskExecutor, void, Args...>
		{
			static RetVal_v1<void> runTaskHelper(const std::string& funName, Args... args)
			{
				CustomTaskExecutor<void, Args...>::runTask(funName, std::forward<Args>(args)...);
				return RetVal_v1<void>{};
			}

			static RetVal_v1<void> runTaskNoThrowHelper(bool& success, const std::string& funName, Args... args)
			{
				success = CustomTaskExecutor<void, Args...>::runTaskNoThrow(funName, std::forward<Args>(args)...);
				return RetVal_v1<void>{};
			}

			template<typename F>
			static RetVal_v1<void> directFunCallHelper(F task, Args... args)
			{
				task(std::forward<Args>(args)...);
				return RetVal_v1<void>{};
			}
		};

		template<template<typename... Args> class CustomTaskExecutor, typename RetType, typename... Args>
		struct TestFunctionStruct
		{
			template<typename ActualFunType>
			static void testFunction(const std::string& funName, ActualFunType task, Args... args)
			{
				bool success1 = false;
				bool success2 = false;

				std::cout << "\n\n" << funName << ":";
				std::cout << "\n" << "addTask: "; success1 = CustomTaskExecutor<RetType, Args...>::addTask(funName, task); std::cout << " success1: " << success1;
				std::cout << "\n" << "addTask: "; success2 = CustomTaskExecutor<RetType, Args...>::addTask(funName, task); std::cout << " success2: " << success2;
				
				std::cout << "\n" << "runTask: ";
				RetVal_v1<RetType> retVal1 = TestFunctionStructHelper<CustomTaskExecutor, RetType, Args...>::runTaskHelper(funName, std::forward<Args>(args)...);
				std::cout << " retVal1: '" << retVal1.toString() << "'";
				
				bool success3 = false;
				std::cout << "\n" << "runTaskNoThrow: ";
				RetVal_v1<RetType> retVal2 = TestFunctionStructHelper<CustomTaskExecutor, RetType, Args...>::runTaskNoThrowHelper(success3, funName, std::forward<Args>(args)...);
				std::cout << " retVal2: '" << retVal2.toString() << "' success3: " << success3;

				std::cout << "\n" << "run actual task to get expected output: ";
				RetVal_v1<RetType> expectedOutput = TestFunctionStructHelper<CustomTaskExecutor, RetType, Args...>::directFunCallHelper(task, std::forward<Args>(args)...);

				cout << "\n\n";
				MM_EXPECT_TRUE(success1 == true);
				MM_EXPECT_TRUE(success2 == false);
				MM_EXPECT_TRUE(retVal1 == expectedOutput, retVal1.toString(), expectedOutput.toString());
				MM_EXPECT_TRUE(success3 == true);
				MM_EXPECT_TRUE(retVal2 == expectedOutput, retVal2.toString(), expectedOutput.toString());

			}
		};

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
			std::string s{ "s-aa" };
			const std::string cs{ "cs-aa" };
			std::string rs{ "rs-aa" };
			std::string crs{ "crs-aa" };
			//std::string rvrs{ "rvrs-aa" };

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
			using TestFunctionStruct5 = TestFunctionStruct<CustomTaskExecutor, std::string, std::string, const std::string, std::string&, const std::string&, std::string&&>;

			//#if defined(_MSC_VER)
			incrementInput(); TestFunctionStruct1::testFunction(funName, cStyleFunVoidVoid);
			incrementInput(); TestFunctionStruct2::testFunction(funName, cStyleFunVoidInt, intIn);
			incrementInput(); TestFunctionStruct3::testFunction(funName, cStyleFunStringDouble, dIn);
			incrementInput(); TestFunctionStruct4::testFunction(funName, cStyleFunStringConstRefStringDoubleLonglong, strIn, dIn, llIn);
			incrementInput(); TestFunctionStruct5::testFunction(funName, cStyleFunStrStrConstStrRefStrConstRefStrRValRefStr, s, cs, rs, crs, "rvrs");
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

		void testAllCustomTaskExecutorVersions_v5()
		{
			testCustomTaskExecutor_v2v4<CustomTaskExecutor_v5::Task>();

			std::cout << std::boolalpha;
			std::string funName;
			int funIndex = 0;
			int intIn = 0;
			long long llIn = 0;
			double dIn = 1.000;
			std::string strIn{ "aa" };
			std::string s{ "s-aa" };
			const std::string cs{ "cs-aa" };
			std::string rs{ "rs-aa" };
			const std::string crs{ "crs-aa" };
			//std::string rvrs{ "rvrs-aa" };

			CustomTaskExecutor_v5::Task<std::string, double>::addTask("fun1", cStyleFunStringDouble);
			s = CustomTaskExecutor_v5::Task<std::string, double>::runTask("fun1", dIn);

			CustomTaskExecutor_v5::Task<std::string, std::string, const std::string, std::string&, const std::string&, std::string&&>::addTask("fun2", cStyleFunStrStrConstStrRefStrConstRefStrRValRefStr);
			s = CustomTaskExecutor_v5::Task<std::string, std::string, const std::string, std::string&, const std::string&, std::string&&>::runTask("fun2", s, cs, rs, crs, "rvrs");

			std::cout << "\nFinished";
		}
	}


	MM_DECLARE_FLAG(TemplateMetaProgrammingCustomTaskExecutor_v5);

	MM_UNIT_TEST(TemplateMetaProgrammingCustomTaskExecutorTest_v5, TemplateMetaProgrammingCustomTaskExecutor_v5)
	{
		customTaskExecutor_v5::testAllCustomTaskExecutorVersions_v5();
	}
}

