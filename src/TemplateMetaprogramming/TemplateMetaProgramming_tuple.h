#pragma once

#include <cstddef>

namespace mm {

	template <typename... Ts> 
	struct tuple
	{
		template<size_t index>
		struct elementType
		{
			typedef void type;
		};
	};

	template <typename T, typename... Ts>
	struct tuple<T, Ts...> : public tuple<Ts...>
	{
	public:
		tuple(T t, Ts... ts) 
			: tuple<Ts...>(ts...), 
			  tail(t)
		{
		}

		template<size_t index>
		struct elementType
		{
			typedef typename tuple<Ts...>::template elementType<index - 1>::type type;
		};

		template<>
		struct elementType<0>
		{
			typedef T type;
		};

		template<size_t index>
		typename tuple<T, Ts...>::template elementType<index>::type getAt()
		//auto getAt()
		{
			tuple<Ts...>& ref = static_cast<tuple<Ts...>>(*this);
			return ref.getAt<index - 1>();
		}

		template<>
		typename tuple<T, Ts...>::template elementType<0>::type  getAt<0>()
		//T getAt<0>()
		{
			return tail;
		}

	protected:
		T tail;
	};

	//g++ does not allow explicit template specialization inside class scope, it should be in enclosing namespace scope
	//template <int index, typename T, typename... Ts>
	////template<int index>
	//struct tuple<T, Ts...>::elementType<index>
	//{
	//	typedef typename tuple<Ts...>::template elementType<index - 1>::type type;
	//};
	
	//template <typename T, typename... Ts>
	////template<>
	//struct tuple<T, Ts...>::elementType<0>
	//{
	//	typedef T type;
	//};

	//g++ does not allow explicit template specialization inside class scope, it should be in enclosing namespace scope
	//template <typename T, typename... Ts>
	//template<int index>
	//typename tuple<T, Ts...>::template elementType<index - 1>::type getAt<index>()
	//	//auto getAt()
	//{
	//	tuple<Ts...>& ref = static_cast<tuple<Ts...>>(*this);
	//	return ref.getAt<index - 1>();
	//}

	//template<>
	//template <typename T, typename... Ts>
	//typename tuple<T, Ts...>::template elementType<0>::type tuple<T, Ts...>::getAt<0>(void)
	//{
	//	return tail;
	//}

}
