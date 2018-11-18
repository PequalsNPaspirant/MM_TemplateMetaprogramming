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
			typedef typename tuple<Ts...>::elementType<index - 1>::type type;
		};

		template<>
		struct elementType<0>
		{
			typedef T type;
		};

		template<size_t index>
		typename tuple<T, Ts...>::elementType<index>::type getAt(void)
		//auto getAt()
		{ 
			return static_cast<tuple<Ts...>>(*this).getAt<index - 1>();
		}

		template<>
		typename tuple<T, Ts...>::elementType<0>::type  getAt<0>(void)
		{
			return tail;
		}

	protected:
		T tail;
	};

}
