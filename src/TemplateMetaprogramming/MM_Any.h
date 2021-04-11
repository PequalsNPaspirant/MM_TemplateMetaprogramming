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
#include<type_traits>
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
		template<typename T>
		MM_Any(std::remove_reference_t<T>&& obj)
		{
			data_ = make_unique< ConcreteType<T> >(std::move(obj));
		}

		MM_Any(const MM_Any& copy)
		{
			data_ = copy.data_->clone();
		}

		MM_Any(MM_Any&& move)
		{
			data_ = std::move(move.data_);
		}

		MM_Any& operator=(const MM_Any& copy)
		{
			if (this == &copy)
				return *this;

			data_ = copy.data_->clone();
			return *this;
		}

		void operator=(MM_Any&& move)
		{
			data_ = std::move(move.data_);
		}

		template<typename T>
		operator T()
		{
			return getValue<T>();
		}

		template<typename T>
		T getValue()
		{
			return data_->getValue<T>();
		}

		template<typename T>
		bool isType()
		{
			return data_->isType<T>();
		}

		void doTask()
		{

		}

	private:
		class AnyType
		{
		public:
			template<typename T>
			T getValue()
			{
				ConcreteType<T>* ct = dynamic_cast<ConcreteType<T>*>(this);
				if (ct != nullptr)
					return ct->getConcreteValue();

				return T{};
			}

			template<typename T>
			bool isType()
			{
				ConcreteType<T>* ct = dynamic_cast<ConcreteType<T>*>(this);
				if (ct != nullptr)
					return true;

				return false;
			}

			virtual unique_ptr<AnyType> clone() = 0;
			virtual void doTask() = 0;
			virtual ~AnyType() {}
		};

		template<typename T>
		class ConcreteType : public AnyType
		{
		public:
			ConcreteType(const T& t)
				: val_{ t }
			{}

			ConcreteType(std::remove_reference_t<T>&& t)
				: val_{ std::move(t) }
			{}

			//template<typename T>
			//void getData(T& n) override
			//{
			//	n = val_;
			//}

			T getConcreteValue()
			{
				return val_;
			}

			unique_ptr<AnyType> clone() override
			{
				return make_unique< ConcreteType<T> >(*this);
			}

			void doTask() override
			{

			}

		private:
			T val_;
		};

		unique_ptr<AnyType> data_;
	};


}