#pragma once

#include <cstddef>
#include <tuple>

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

#if defined(_MSC_VER)

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

#elif defined(__GNUC__)

		template<size_t index, bool Dummy = true>
		struct elementType
		{
			typedef typename tuple<Ts...>::template elementType<index - 1>::type type;
		};

		template<bool Dummy>
		struct elementType<0, Dummy>
		{
			typedef T type;
		};

/*		template<size_t index>
		typename tuple<T, Ts...>::template elementType<index>::type getAt()
		{
			return getAtInternal<index, true>();
		};*/

		//template<size_t index>
		//typename tuple<T, Ts...>::template elementType<index>::type getAt()
		//auto getAt()
		//{
		//	const tuple<Ts...>& ref = static_cast<tuple<Ts...>>(*this);
		//	return ref.getAt<index - 1>();
		//}

		//template<typename Dummy>
		//typename tuple<T, Ts...>::template elementType<0>::type getAt<0, Dummy>()
		//T getValue()
		//{
		//	return tail;
		//}

#endif

	protected:
		T tail;
	};

}
