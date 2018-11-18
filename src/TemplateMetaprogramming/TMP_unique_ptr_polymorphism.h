#include <memory>
#include <iostream>
using namespace std;

namespace mm {

	class A 
	{
	public:
		~A() {
			std::cout << "A::~A()" << std::endl;
		}
	};

	class BA : public A 
	{
	public:
		~BA() 
		{
			std::cout << "BA::~BA()" << std::endl;
		}
	};

	struct Filler 
	{
		char filler[16];
	};

	class CBA : private Filler, public BA 
	{
	public:
		CBA() 
		{
			std::cout << "CBA::CBA()" << std::endl;
		}
		~CBA() 
		{
			std::cout << "CBA::~CBA()" << std::endl;
		}
	};



	template<typename T> struct Deleter 
	{
		Deleter()
			: fun(funDeleter<T>)
		{

		}
		template <typename C>
		Deleter(Deleter<C>&& obj)
			: fun(obj.fun)
		{

		}
		void operator()(T* obj)
		{
			(*fun)(obj);
		}

		template<typename C>
		static void funDeleter(void* obj)
		{
			delete (static_cast<C*>(obj));
		}

		typedef void(*fPtr)(void*);
		fPtr fun;
	};

}