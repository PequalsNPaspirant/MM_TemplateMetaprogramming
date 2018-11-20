#include <memory>
#include <iostream>
using namespace std;

namespace mm {

	class A 
	{
	public:
		~A() {
			std::cout << "\nA::~A()";
		}
	};

	class BA : public A 
	{
	public:
		~BA() 
		{
			std::cout << "\nBA::~BA()";
		}
	};

	class CBA : public BA 
	{
	public:
		CBA() 
		{
			std::cout << "\nCBA::CBA()";
		}
		~CBA() 
		{
			std::cout << "\nCBA::~CBA()";
		}
	};

	struct Filler 
	{
		char filler[16];
	};

	class CBAF : public BA, private Filler
	{
	public:
		CBAF() 
		{
			std::cout << "\nCBAF::CBAF()";
		}
		~CBAF() 
		{
			std::cout << "\nCBAF::~CBAF()";
		}
	};


	class FCBA : private Filler, public BA 
	{
	public:
		FCBA() 
		{
			std::cout << "\nFCBA::FCBA()";
		}
		~FCBA() 
		{
			std::cout << "\nFCBA::~FCBA()";
		}
	};

	template<typename T> 
	struct Deleter_v1
	{
		Deleter_v1()
			: fun(Deleter_v1::funDeleter<T>)
		{

		}
		template <typename C>
		Deleter_v1(Deleter_v1<C>&& obj)
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

	
	struct ControlBlockBase
	{
		virtual void destroy(void* obj) = 0;
		virtual ~ControlBlockBase() = default;
	};

	template<typename T>
	struct ControlBlock : public ControlBlockBase
	{
		ControlBlock()
			: fun(ControlBlock::funDeleter)
		{
		}

		~ControlBlock()
		{

		}

		void destroy(void* obj) override
		{
			(*fun)(static_cast<T*>(obj));
		}

		static void funDeleter(T* obj)
		{
			delete obj;
		}

		typedef void(*fPtr)(T*);
		fPtr fun;
	};

	template<typename T> 
	struct Deleter_v2
	{
		Deleter_v2()
			: pObj_(new ControlBlock<T>())
		{

		}
		~Deleter_v2()
		{
			delete pObj_;
		}

		template <typename C>
		Deleter_v2(Deleter_v2<C>&& obj)
			: pObj_(obj.pObj_)
		{
			obj.pObj_ = nullptr;
		}
		void operator()(T* obj)
		{
			pObj_->destroy(obj);
		}

		ControlBlockBase* pObj_;
	};
	
}
