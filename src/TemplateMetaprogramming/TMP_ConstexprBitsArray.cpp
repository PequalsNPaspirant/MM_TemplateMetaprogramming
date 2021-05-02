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
			constexpr constexprBitsArray<size_t, 100> bits100(true);

			constexpr size_t e1 = bits100.getAt<0>();
			static_assert(e1 == 0b1111111111111111111111111111111111111111111111111111111111111111, "Unexpected initialization of object");
			MM_EXPECT_TRUE(e1 == 0b1111111111111111111111111111111111111111111111111111111111111111, e1);
			constexpr size_t e2 = bits100.getAt<1>();
			static_assert(e2 == 0b1111111111111111111111111111111111110000000000000000000000000000, "Unexpected initialization of object");
			MM_EXPECT_TRUE(e2 == 0b1111111111111111111111111111111111110000000000000000000000000000, e2);
			//constexpr size_t e3 = bits100.getAt<2>(); //fails to compile: error: array subscript value ‘2’ is outside the bounds of array type ‘long unsigned int [2]’

			constexpr bool flag1 = bits100.getBit(0);
			static_assert(flag1 == true, "Unexpected initialization of object");
			MM_EXPECT_TRUE(flag1 == true, flag1);
			constexpr bool flag2 = bits100.getBit(99);
			static_assert(flag2 == true, "Unexpected initialization of object");
			MM_EXPECT_TRUE(flag2 == true, flag2);
			constexpr bool flag3 = bits100.getBit(100); //does not fail to compile because the bitIndex upto 128 is valid and is initialized for this object
			//constexpr bool flag4 = bits100.getBit(129); //fails to compile  error: array subscript value ‘2’ is outside the bounds of array type ‘long unsigned int [2]’

			constexpr constexprBitsArray<size_t, 90> bits90;

			//constexpr constexprBitsArray<int, 90> bits80(true); //fails to compile since the type is not unsigned
		}

	}

	MM_DECLARE_FLAG(constexprBitsArray_v1);

	MM_UNIT_TEST(constexprBitsArray_v1_test1, constexprBitsArray_v1)
	{
		constexprBitsArray_v1::test();
	}
}

