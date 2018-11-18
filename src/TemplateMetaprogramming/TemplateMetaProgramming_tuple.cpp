
#include <typeinfo>

#include <TemplateMetaprogramming/TemplateMetaProgramming_tuple.h>
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	MM_DECLARE_FLAG(TemplateMetaProgramming_tuple)

	MM_UNIT_TEST(TemplateMetaProgramming_tuple_test_1, TemplateMetaProgramming_tuple)
	{
		tuple<double, uint64_t, const char*> t1(12.2, 42, "big");
		/*
		// Ignoring the constructor, here's a pseudo-trace of the tuple structs created:
		struct tuple<double, uint64_t, const char*> : tuple<uint64_t, const char*> {
		  double tail;
		}

		struct tuple<uint64_t, const char*> : tuple<const char*> {
		  uint64_t tail;
		}

		struct tuple<const char*> : tuple {
		  const char* tail;
		}

		struct tuple {
		}

		// The layout of data members in the original 3-element tuple will be:
		[const char* tail, uint64_t tail, double tail]
		*/

		MM_EXPECT_TRUE(typeid(tuple<double, uint64_t, const char*>::elementType<0>::type) == typeid(double))
		MM_EXPECT_TRUE(typeid(tuple<double, uint64_t, const char*>::elementType<1>::type) == typeid(uint64_t))
		MM_EXPECT_TRUE(typeid(tuple<double, uint64_t, const char*>::elementType<2>::type) == typeid(const char*))

		MM_EXPECT_TRUE(fabs(t1.getAt<0>() - 12.2) < 0.0001, t1.getAt<0>())
		MM_EXPECT_TRUE(t1.getAt<1>() == 42, t1.getAt<1>())
		MM_EXPECT_TRUE(t1.getAt<2>() == "big", t1.getAt<2>())

		MM_EXPECT_TRUE(typeid(tuple<double, uint64_t, const char*>::elementType<0>::type) == typeid(t1.getAt<0>()))
		MM_EXPECT_TRUE(typeid(tuple<double, uint64_t, const char*>::elementType<1>::type) == typeid(t1.getAt<1>()))
		MM_EXPECT_TRUE(typeid(tuple<double, uint64_t, const char*>::elementType<2>::type) == typeid(t1.getAt<2>()))
	}
}