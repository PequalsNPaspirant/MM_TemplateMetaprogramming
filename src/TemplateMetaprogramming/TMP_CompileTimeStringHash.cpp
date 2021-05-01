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
#include "TMP_CompileTimeStringHash.h"

namespace mm {

	namespace compileTimeStringHash_v1 {


		void test()
		{

		}

	}

	MM_DECLARE_FLAG(compileTimeStringHash_v1);

	MM_UNIT_TEST(compileTimeStringHash_v1_test1, compileTimeStringHash_v1)
	{
		compileTimeStringHash_v1::test();
	}
}

