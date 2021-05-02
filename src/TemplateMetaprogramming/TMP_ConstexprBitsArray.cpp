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
			constexprBitsArray<size_t, 100> bits100(true);
			//bits100.setAllBits(true);

			size_t e1 = bits100.getAt<0>();
			MM_EXPECT_TRUE(e1 == 0b1111111111111111111111111111111111111111111111111111111111111111, e1);
			size_t e2 = bits100.getAt<1>();
			MM_EXPECT_TRUE(e2 == 0b1111111111111111111111111111111111110000000000000000000000000000, e2);
			
			bool flag1 = bits100.getBit(0);
			MM_EXPECT_TRUE(flag1 == true, flag1);
			bool flag2 = bits100.getBit(99);
			MM_EXPECT_TRUE(flag2 == true, flag2);

			constexprBitsArray<size_t, 90> bits90;

			//constexprBitsArray<int, 90> bits80(true); //fails to compile since the type is not unsigned
		}

	}

	MM_DECLARE_FLAG(constexprBitsArray_v1);

	MM_UNIT_TEST(constexprBitsArray_v1_test1, constexprBitsArray_v1)
	{
		constexprBitsArray_v1::test();
	}
}

