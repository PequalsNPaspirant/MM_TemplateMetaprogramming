

#include "TemplateMetaprogramming/TMP_unique_ptr_polymorphism.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
//#include "Assert/MyAssert.h"

namespace mm {

	template<typename T> using SharedPtr = std::shared_ptr<T>;
	template<typename T> using UniquePtr_v1 = std::unique_ptr<T, Deleter_v1<T>>;
	template<typename T> using UniquePtr_v2 = std::unique_ptr<T, Deleter_v2<T>>;

	MM_DECLARE_FLAG(TMP_unique_ptr_polymorphism);

	MM_UNIT_TEST(TMP_unique_ptr_polymorphism_test_1, TMP_unique_ptr_polymorphism)
	{
		cout << "\n\n----------- Testing CBA";
		{
			//This works fine
			cout << "\n    with SharedPtr";			
			auto cbaPtr = SharedPtr<CBA>(new CBA);
			SharedPtr<BA> baPtr = std::move(cbaPtr);
			SharedPtr<A> aPtr = std::move(baPtr);
		}

		{
			//This works fine
			cout << "\n    with UniquePtr_v1";			
			auto cbaPtr1 = UniquePtr_v1<CBA>(new CBA);
			UniquePtr_v1<BA> baPtr1 = std::move(cbaPtr1);
			UniquePtr_v1<A> aPtr1 = std::move(baPtr1);
		}
		
		{
			//This works fine
			cout << "\n    with UniquePtr_v2";			
			auto cbaPtr2 = UniquePtr_v2<CBA>(new CBA);
			UniquePtr_v2<BA> baPtr2 = std::move(cbaPtr2);
			UniquePtr_v2<A> aPtr2 = std::move(baPtr2);
		}

		cout << "\n\n----------- Testing CBAF";
		{
			//This works fine as the shared_ptr stores the type eraser along with pointer to object
			cout << "\n    with SharedPtr";			
			auto cbaPtr1 = SharedPtr<CBAF>(new CBAF);
			SharedPtr<BA> baPtr1 = std::move(cbaPtr1);
			SharedPtr<A> aPtr1 = std::move(baPtr1);
		}

		{
			//This works fine as the Filler is at the end of CBAF object
			cout << "\n    with UniquePtr_v1";			
			auto cbaPtr1 = UniquePtr_v1<CBAF>(new CBAF);
			UniquePtr_v1<BA> baPtr1 = std::move(cbaPtr1);
			UniquePtr_v1<A> aPtr1 = std::move(baPtr1);
		}
		
		{
			//This works fine as the Filler is at the end of CBAF object
			cout << "\n    with UniquePtr_v2";			
			auto cbaPtr2 = UniquePtr_v2<CBAF>(new CBAF);
			UniquePtr_v2<BA> baPtr2 = std::move(cbaPtr2);
			UniquePtr_v2<A> aPtr2 = std::move(baPtr2);
		}

		cout << "\n\n----------- Testing FCBA";
		{
			//This works fine as the shared_ptr stores the type eraser along with pointer to object
			cout << "\n    with SharedPtr";			
			auto cbaPtr1 = SharedPtr<FCBA>(new FCBA);
			SharedPtr<BA> baPtr1 = std::move(cbaPtr1);
			SharedPtr<A> aPtr1 = std::move(baPtr1);
		}

		{
			//This crashes
			cout << "\n    with UniquePtr_v1: this is going to crash as the FCBA* is cast into BA* and A* and then back to FCBA*";
			//auto cbaPtr1 = UniquePtr_v1<FCBA>(new FCBA);
			//UniquePtr_v1<BA> baPtr1 = std::move(cbaPtr1);
			//UniquePtr_v1<A> aPtr1 = std::move(baPtr1);
		}
		
		{
			//This crashes
			cout << "\n    with UniquePtr_v2: this is going to crash as the FCBA* is cast into BA* and A* and then back to FCBA*";
			//auto cbaPtr2 = UniquePtr_v2<FCBA>(new FCBA);
			//UniquePtr_v2<BA> baPtr2 = std::move(cbaPtr2);
			//UniquePtr_v2<A> aPtr2 = std::move(baPtr2);
		}

		//MM_EXPECT_TRUE(true);
	}
}
