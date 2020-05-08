

#include "TemplateMetaprogramming/MM_Any.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
//#include "Assert/MyAssert.h"

namespace mm {

	MM_DECLARE_FLAG(MM_Any_type);

	MM_UNIT_TEST(MM_Any_type_test_1, MM_Any_type)
	{
		//Test MM_Any class
		int i = 10;
		long long ll = 20;
		double d = 3.142;
		class A {};
		A a;

		MM_Any iObj{ i };
		MM_Any llObj{ ll };
		MM_Any dObj{ d };
		MM_Any aObj{ a };

		int i2;
		i2 = iObj;
		i2 = llObj;
		i2 = dObj;
		i2 = aObj;

	}



	class B {};
	class D : public B {};

	class Base
	{
	public:
		virtual int covarintVirtulFun1() = 0;
		virtual B* covarintVirtulFun2() = 0;

		virtual Base* clone()
		{
			return nullptr;
		}
	};

	class Derived : public Base
	{
	public:
		int covarintVirtulFun1() override
		{
			return 0;
		}

		D* covarintVirtulFun2() override
		{
			return nullptr;
		}

		Derived* clone()
		{
			return nullptr;
		}
	};

	//TODO: MOve it to some other project
	MM_DECLARE_FLAG(MM_returnTypeCovariance);

	MM_UNIT_TEST(MM_returnTypeCovariance_test_1, MM_returnTypeCovariance)
	{
		{
			Base* b = new Derived;
			b->covarintVirtulFun1();
			//D* d1 = b->covarintVirtulFun2();
			//D* d2 = b->clone();
			Derived* d = new Derived;
			D* d1 = d->covarintVirtulFun2();
			Derived* d2 = d->clone();
		}
	}


}	