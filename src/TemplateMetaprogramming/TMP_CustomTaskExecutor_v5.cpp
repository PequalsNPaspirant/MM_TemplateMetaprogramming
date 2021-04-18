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

			std::string operator()(
				std::string s, const std::string cs, std::string& rs, const std::string& crs, std::string&& rvrs)
			{
				std::cout << "std::string operator()("
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
				return std::string{ "Args: " } +s + "," + cs + "," + rs + "," + crs + "," + rvrs;
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
		auto lambdaStrStrConstStrRefStrConstRefStrRValRefStr = [](std::string s, const std::string cs, std::string& rs, const std::string& crs, std::string&& rvrs) -> std::string
		{
			std::cout << "std::string lambdaStrStrConstStrRefStrConstRefStrRValRefStr("
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
			return std::string{ "Args: " } +s + "," + cs + "," + rs + "," + crs + "," + rvrs;
		};

		//std::functions
		std::function<void(void)> stdFunVoidVoid = []()
		{
			std::cout << "stdFunVoidVoid()";
		};
		std::function<void(int)> stdFunVoidInt = [](int n)
		{
			std::cout << "stdFunVoidInt(int n) Args: n: " << n;
		};
		std::function<std::string(double)> stdFunStringDouble = [](double d) -> std::string
		{
			std::cout << "std::string stdFunStringDouble(double d) Args: d: " << d;
			return std::string{ "Args: " } +std::to_string(d);
		};
		std::function<std::string(const std::string&, double, long long)> stdFunStringConstRefStringDoubleLonglong 
			= [](const std::string& s, double d, long long ll) -> std::string
		{
			std::cout << "std::string stdFunStringConstRefStringDoubleLonglong(const std::string& s, double d, long long ll) Args: s: " << s << " d: " << d << " ll: " << ll;
			return std::string{ "Args: " } +s + "," + std::to_string(d) + "," + std::to_string(ll);
		};
		std::function<std::string(std::string, const std::string, std::string&, const std::string&, std::string&&)> stdFunStrStrConstStrRefStrConstRefStrRValRefStr
			= [](std::string s, const std::string cs, std::string& rs, const std::string& crs, std::string&& rvrs) -> std::string
		{
			std::cout << "std::string stdFunStrStrConstStrRefStrConstRefStrRValRefStr("
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
			return std::string{ "Args: " } +s + "," + cs + "," + rs + "," + crs + "," + rvrs;
		};


		// =========== Helper Classes / Functions ===========

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
			static RetVal_v1<RetType> testRunTask(const std::string& funName, Args... args)
			{
				std::cout << "\n" << "runTask: ";
				RetVal_v1<RetType> retVal = TestFunctionStructHelper<CustomTaskExecutor, RetType, Args...>::runTaskHelper(funName, std::forward<Args>(args)...);
				std::cout << " retVal: '" << retVal.toString() << "'";
				return retVal;
			}

			static RetVal_v1<RetType> testRunTaskNoThrow(bool& success3, const std::string& funName, Args... args)
			{
				success3 = false;
				std::cout << "\n" << "runTaskNoThrow: ";
				RetVal_v1<RetType> retValNoThrow = TestFunctionStructHelper<CustomTaskExecutor, RetType, Args...>::runTaskNoThrowHelper(success3, funName, std::forward<Args>(args)...);
				std::cout << " retValNoThrow: '" << retValNoThrow.toString() << "' success3: " << success3;
				return retValNoThrow;
			}

			template<typename ActualFunType>
			static RetVal_v1<RetType> testAddTaskAndDirectFunCall(bool& success1, bool& success2, const std::string& funName, ActualFunType task, Args... args)
			{
				success1 = false;
				success2 = false;

				std::cout << "\n\n" << funName << ":";
				std::cout << "\n" << "addTask: "; success1 = CustomTaskExecutor<RetType, Args...>::addTask(funName, task); std::cout << " success1: " << success1;
				std::cout << "\n" << "addTask: "; success2 = CustomTaskExecutor<RetType, Args...>::addTask(funName, task); std::cout << " success2: " << success2;

				std::cout << "\n" << "run actual task to get expected output: ";
				RetVal_v1<RetType> retValExpected = TestFunctionStructHelper<CustomTaskExecutor, RetType, Args...>::directFunCallHelper(task, std::forward<Args>(args)...);
				std::cout << " retValExpected: '" << retValExpected.toString() << "'";

				return retValExpected;
			}
		};

		struct InputData
		{
			int intIn;
			long long llIn;
			double dIn;
			std::string strIn;
			std::string s;
			const std::string cs;
			//std::string& rs; 
			std::string rs;
			const std::string& crs;
			std::string rvrs;

			bool operator==(const InputData& rhs)
			{
				return
					intIn == rhs.intIn &&
					llIn == rhs.llIn &&
					dIn == rhs.dIn &&
					strIn == rhs.strIn &&
					s == rhs.s &&
					cs == rhs.cs &&
					rs == rhs.rs &&
					crs == rhs.crs &&
					rvrs == rhs.rvrs;
			}
		};

		template<typename RetType>
		void verifyResults(bool success1, bool success2, bool success3,
			RetVal_v1<RetType> retValExpected, RetVal_v1<RetType> retVal, RetVal_v1<RetType> retValNoThrow,
			InputData in1, InputData in2, InputData in3) 
		{
			cout << "\n\n";
			MM_EXPECT_TRUE(success1 == true);
			MM_EXPECT_TRUE(success2 == false);
			MM_EXPECT_TRUE(retVal == retValExpected, retVal.toString(), retValExpected.toString());
			MM_EXPECT_TRUE(success3 == true);
			MM_EXPECT_TRUE(retValNoThrow == retValExpected, retValNoThrow.toString(), retValExpected.toString());
			MM_EXPECT_TRUE(in1 == in2);
			MM_EXPECT_TRUE(in2 == in3);
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
			std::string cs{ "cs-aa" };
			std::string rs{ "rs-aa" };
			std::string crs{ "crs-aa" };
			std::string rvrs{ "rvrs-aa" };

			auto incrementstring = [](std::string& strIn) {
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

			auto incrementInput = [&]() -> InputData {
				intIn += 1;
				funName = "fun_" + std::to_string(intIn);
				llIn += 10;
				dIn += 0.001;
				incrementstring(strIn);
				incrementstring(s);
				incrementstring(cs);
				incrementstring(rs);
				incrementstring(crs);
				incrementstring(rvrs);

				return InputData{ intIn, llIn, dIn, strIn, s, cs, rs, crs, rvrs };
			};

			using TestFunctionStruct1 = TestFunctionStruct<CustomTaskExecutor, void>;
			using TestFunctionStruct2 = TestFunctionStruct<CustomTaskExecutor, void, int>;
			using TestFunctionStruct3 = TestFunctionStruct<CustomTaskExecutor, std::string, double>;
			using TestFunctionStruct4 = TestFunctionStruct<CustomTaskExecutor, std::string, const std::string&, double, long long>;
			using TestFunctionStruct5 = TestFunctionStruct<CustomTaskExecutor, std::string, std::string, const std::string, std::string&, const std::string&, std::string&&>;

			bool success1, success2, success3;

			//Test c-style functions
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct1::testAddTaskAndDirectFunCall(success1, success2, funName, cStyleFunVoidVoid);
				RetVal_v1<void> retVal = TestFunctionStruct1::testRunTask(funName);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct1::testRunTaskNoThrow(success3, funName);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct2::testAddTaskAndDirectFunCall(success1, success2, funName, cStyleFunVoidInt, in1.intIn);
				RetVal_v1<void> retVal = TestFunctionStruct2::testRunTask(funName, in2.intIn);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct2::testRunTaskNoThrow(success3, funName, in3.intIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct3::testAddTaskAndDirectFunCall(success1, success2, funName, cStyleFunStringDouble, in1.dIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct3::testRunTask(funName, in2.dIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct3::testRunTaskNoThrow(success3, funName, in3.dIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct4::testAddTaskAndDirectFunCall(success1, success2, funName, cStyleFunStringConstRefStringDoubleLonglong, in1.strIn, in1.dIn, in1.llIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct4::testRunTask(funName, in2.strIn, in2.dIn, in2.llIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct4::testRunTaskNoThrow(success3, funName, in3.strIn, in3.dIn, in3.llIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct5::testAddTaskAndDirectFunCall(success1, success2, funName, cStyleFunStrStrConstStrRefStrConstRefStrRValRefStr, in1.s, in1.cs, in1.rs, in1.crs, std::move(in1.rvrs));
				RetVal_v1<std::string> retVal = TestFunctionStruct5::testRunTask(funName, in2.s, in2.cs, in2.rs, in2.crs, std::move(in2.rvrs));
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct5::testRunTaskNoThrow(success3, funName, in3.s, in3.cs, in3.rs, in3.crs, std::move(in3.rvrs));
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}

			//Test functors
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct1::testAddTaskAndDirectFunCall(success1, success2, funName, Functor{});
				RetVal_v1<void> retVal = TestFunctionStruct1::testRunTask(funName);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct1::testRunTaskNoThrow(success3, funName);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct2::testAddTaskAndDirectFunCall(success1, success2, funName, Functor{}, in1.intIn);
				RetVal_v1<void> retVal = TestFunctionStruct2::testRunTask(funName, in2.intIn);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct2::testRunTaskNoThrow(success3, funName, in3.intIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct3::testAddTaskAndDirectFunCall(success1, success2, funName, Functor{}, in1.dIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct3::testRunTask(funName, in2.dIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct3::testRunTaskNoThrow(success3, funName, in3.dIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct4::testAddTaskAndDirectFunCall(success1, success2, funName, Functor{}, in1.strIn, in1.dIn, in1.llIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct4::testRunTask(funName, in2.strIn, in2.dIn, in2.llIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct4::testRunTaskNoThrow(success3, funName, in3.strIn, in3.dIn, in3.llIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct5::testAddTaskAndDirectFunCall(success1, success2, funName, Functor{}, in1.s, in1.cs, in1.rs, in1.crs, std::move(in1.rvrs));
				RetVal_v1<std::string> retVal = TestFunctionStruct5::testRunTask(funName, in2.s, in2.cs, in2.rs, in2.crs, std::move(in2.rvrs));
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct5::testRunTaskNoThrow(success3, funName, in3.s, in3.cs, in3.rs, in3.crs, std::move(in3.rvrs));
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}

			//Test lambdas
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct1::testAddTaskAndDirectFunCall(success1, success2, funName, lambdaVoidVoid);
				RetVal_v1<void> retVal = TestFunctionStruct1::testRunTask(funName);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct1::testRunTaskNoThrow(success3, funName);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct2::testAddTaskAndDirectFunCall(success1, success2, funName, lambdaVoidInt, in1.intIn);
				RetVal_v1<void> retVal = TestFunctionStruct2::testRunTask(funName, in2.intIn);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct2::testRunTaskNoThrow(success3, funName, in3.intIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct3::testAddTaskAndDirectFunCall(success1, success2, funName, lambdaStringDouble, in1.dIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct3::testRunTask(funName, in2.dIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct3::testRunTaskNoThrow(success3, funName, in3.dIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct4::testAddTaskAndDirectFunCall(success1, success2, funName, lambdaStringConstRefStringDoubleLonglong, in1.strIn, in1.dIn, in1.llIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct4::testRunTask(funName, in2.strIn, in2.dIn, in2.llIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct4::testRunTaskNoThrow(success3, funName, in3.strIn, in3.dIn, in3.llIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct5::testAddTaskAndDirectFunCall(success1, success2, funName, lambdaStrStrConstStrRefStrConstRefStrRValRefStr, in1.s, in1.cs, in1.rs, in1.crs, std::move(in1.rvrs));
				RetVal_v1<std::string> retVal = TestFunctionStruct5::testRunTask(funName, in2.s, in2.cs, in2.rs, in2.crs, std::move(in2.rvrs));
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct5::testRunTaskNoThrow(success3, funName, in3.s, in3.cs, in3.rs, in3.crs, std::move(in3.rvrs));
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}

			//Test std::function
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct1::testAddTaskAndDirectFunCall(success1, success2, funName, stdFunVoidVoid);
				RetVal_v1<void> retVal = TestFunctionStruct1::testRunTask(funName);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct1::testRunTaskNoThrow(success3, funName);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<void> retValExpected = TestFunctionStruct2::testAddTaskAndDirectFunCall(success1, success2, funName, stdFunVoidInt, in1.intIn);
				RetVal_v1<void> retVal = TestFunctionStruct2::testRunTask(funName, in2.intIn);
				RetVal_v1<void> retValNoThrow = TestFunctionStruct2::testRunTaskNoThrow(success3, funName, in3.intIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct3::testAddTaskAndDirectFunCall(success1, success2, funName, stdFunStringDouble, in1.dIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct3::testRunTask(funName, in2.dIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct3::testRunTaskNoThrow(success3, funName, in3.dIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct4::testAddTaskAndDirectFunCall(success1, success2, funName, stdFunStringConstRefStringDoubleLonglong, in1.strIn, in1.dIn, in1.llIn);
				RetVal_v1<std::string> retVal = TestFunctionStruct4::testRunTask(funName, in2.strIn, in2.dIn, in2.llIn);
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct4::testRunTaskNoThrow(success3, funName, in3.strIn, in3.dIn, in3.llIn);
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}
			{
				InputData in1 = incrementInput(); InputData in2 = in1; InputData in3 = in1;
				RetVal_v1<std::string> retValExpected = TestFunctionStruct5::testAddTaskAndDirectFunCall(success1, success2, funName, stdFunStrStrConstStrRefStrConstRefStrRValRefStr, in1.s, in1.cs, in1.rs, in1.crs, std::move(in1.rvrs));
				RetVal_v1<std::string> retVal = TestFunctionStruct5::testRunTask(funName, in2.s, in2.cs, in2.rs, in2.crs, std::move(in2.rvrs));
				RetVal_v1<std::string> retValNoThrow = TestFunctionStruct5::testRunTaskNoThrow(success3, funName, in3.s, in3.cs, in3.rs, in3.crs, std::move(in3.rvrs));
				verifyResults(success1, success2, success3, retValExpected, retVal, retValNoThrow, in1, in2, in3);
			}

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

			//Some manual tests
			cout << "\n\n";

			CustomTaskExecutor_v5::Task<std::string, double>::addTask("fun1", cStyleFunStringDouble);
			std::string s1 = CustomTaskExecutor_v5::Task<std::string, double>::runTask("fun1", dIn);

			CustomTaskExecutor_v5::Task<std::string, std::string, const std::string, std::string&, const std::string&, std::string&&>::addTask("fun2", cStyleFunStrStrConstStrRefStrConstRefStrRValRefStr);
			std::string s2 = CustomTaskExecutor_v5::Task<std::string, std::string, const std::string, std::string&, const std::string&, std::string&&>::runTask("fun2", s, cs, rs, crs, "rvrs");

			std::cout << "\nFinished";
		}
	}


	MM_DECLARE_FLAG(TemplateMetaProgrammingCustomTaskExecutor_v5);

	MM_UNIT_TEST(TemplateMetaProgrammingCustomTaskExecutorTest_v5, TemplateMetaProgrammingCustomTaskExecutor_v5)
	{
		customTaskExecutor_v5::testAllCustomTaskExecutorVersions_v5();
	}
}

