

#include "TemplateMetaprogramming/TMP_unique_ptr_polymorphism.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Assert/MyAssert.h"

namespace mm {

	template<typename T> using UniquePtr = std::unique_ptr<T, Deleter<T>>;

	MM_DECLARE_FLAG(TMP_unique_ptr_polymorphism);

	MM_UNIT_TEST(TMP_unique_ptr_polymorphism_test_1, TMP_unique_ptr_polymorphism)
	{
		auto cbaPtr = UniquePtr<CBA>(new CBA);
		UniquePtr<BA> baPtr = std::move(cbaPtr);
		UniquePtr<A> aPtr = std::move(baPtr);

		MM_EXPECT_TRUE(true);
	}

}