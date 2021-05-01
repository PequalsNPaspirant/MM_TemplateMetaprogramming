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
#include "TMP_ConstexprBitsArray.h"

namespace mm {

	namespace constexprBitsArray_v1 {


		void test()
		{

		}

	}

	MM_DECLARE_FLAG(constexprBitsArray_v1);

	MM_UNIT_TEST(constexprBitsArray_v1_test1, constexprBitsArray_v1)
	{
		constexprBitsArray_v1::test();
	}
}

