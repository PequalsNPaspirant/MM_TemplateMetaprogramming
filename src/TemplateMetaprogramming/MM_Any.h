/*
Reference:
http://www.cplusplus.com/reference/type_traits/
https://en.cppreference.com/w/cpp/types
*/

#include <iostream>
#include <type_traits>
#include <vector>
#include <map>
#include <memory>
using namespace std;

namespace mm{

	class MM_Any
	{
	public:
		template<typename T>
		MM_Any(const T& obj)
		{
			data_ = make_unique< ConcreteType<T> >(obj);
		}
		//template<typename T>
		//MM_Any(T&& obj)
		//{
		//	data_ = make_unique< ConcreteType<T> >(std::move(obj));
		//}

		MM_Any(const MM_Any& copy)
		{

		}

		//MM_Any(MM_Any&& move)
		//{

		//}

		MM_Any& operator=(const MM_Any& copy)
		{
			return *this;
		}

		void operator=(MM_Any&& move)
		{

		}

		template<typename T>
		operator T()
		{
			return data_->getData<T>();
		}

	private:
		class AnyType
		{
		public:
			template<typename T>
			T getData()
			{
				ConcreteType<T>* ct = dynamic_cast<ConcreteType<T>*>(this);
				if (ct != nullptr)
					return ct->getConcreteData();

				return T{};
			}

			virtual ~AnyType() {}
		};

		template<typename T>
		class ConcreteType : public AnyType
		{
		public:
			ConcreteType(const T& t)
				: val_{ t }
			{}

			//ConcreteType(T&& t)
			//	: val_{ std::move(t) }
			//{}

			//template<typename T>
			//void getData(T& n) override
			//{
			//	n = val_;
			//}

			T getConcreteData()
			{
				return val_;
			}

		private:
			T val_;
		};

		unique_ptr<AnyType> data_;
	};


}