

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

		//Test Construction from l-value
		MM_Any iObj{ i };
		MM_Any llObj{ ll };
		MM_Any dObj{ d };
		MM_Any aObj{ a };

		//Test Construction from r-value

		//Test operator T()
		int i2;
		i2 = iObj;
		i2 = llObj;
		i2 = dObj;
		i2 = aObj;
		//Test function getValue<T>()
		i2 = iObj.getValue<int>();
		i2 = llObj.getValue<int>();;
		i2 = dObj.getValue<int>();;
		i2 = aObj.getValue<int>();;

		long long ll2;
		ll2 = iObj;
		ll2 = llObj;
		ll2 = dObj;
		ll2 = aObj;
		ll2 = iObj.getValue<long long>();
		ll2 = llObj.getValue<long long>();
		ll2 = dObj.getValue<long long>();
		ll2 = aObj.getValue<long long>();

		double d2;
		d2 = iObj;
		d2 = llObj;
		d2 = dObj;
		d2 = aObj;
		d2 = iObj.getValue<double>();
		d2 = llObj.getValue<double>();
		d2 = dObj.getValue<double>();
		d2 = aObj.getValue<double>();

		A a2;
		a2 = iObj;
		a2 = llObj;
		a2 = dObj;
		a2 = aObj;
		a2 = iObj.getValue<A>();
		a2 = llObj.getValue<A>();
		a2 = dObj.getValue<A>();
		a2 = aObj.getValue<A>();

		//Test copy constructor
		MM_Any iObj2{ iObj };
		MM_Any llObj2{ llObj };
		MM_Any dObj2{ dObj };
		MM_Any aObj2{ aObj };

		//Test move constructor

		//Test copy assignment

		//Test move assignment

		//Test function isType<T>()


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

	//TODO: Move it to some other project
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