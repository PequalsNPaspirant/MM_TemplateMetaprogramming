/*
Reference:
http://www.cplusplus.com/reference/type_traits/
https://en.cppreference.com/w/cpp/types
*/

#include <iostream>
#include <type_traits>
#include <vector>
#include <map>
using namespace std;

namespace mm{

	//------------------------------------------------------------------

	//generic template
	template<bool expression>
	struct MM_is_true
	{
		static const bool value = false;
	};

	//partial/complete specialization for true
	template<>
	struct MM_is_true<true>
	{
		static const bool value = true;
	};

	//define constexpr global const object for value
	template<bool expression>
	constexpr const bool MM_is_true_v = MM_is_true<expression>::value;

	void test_MM_is_true()
	{
		static_assert(MM_is_true<2 == 2>::value == true, "");
		static_assert(MM_is_true<2 == 4>::value == false, "");
		static_assert(MM_is_true_v<2 == 4> == false, "");
	}

	//------------------------------------------------------------------

	//generic template
	template<typename T1, typename T2>
	struct MM_is_same
	{
		static const bool value = false;
	};

	//partial/complete specialization for true
	template<typename T1>
	struct MM_is_same<T1, T1>
	{
		static const bool value = true;
	};

	//define constexpr global const object for value
	template<typename T1, typename T2>
	constexpr const bool MM_is_same_v = MM_is_same<T1, T2>::value;

	void test_MM_is_same()
	{
		static_assert(MM_is_same<int, int>::value == true, "");
		static_assert(MM_is_same<int, int&>::value == false, "");
		static_assert(MM_is_same<unsigned long, long>::value == false, "");
		static_assert(MM_is_same_v<decltype(2U), decltype(4ULL)> == false, "");
	}

	//------------------------------------------------------------------

	//generic template
	template<typename T> 
	struct MM_remove_const          
	{ 
		//typedef T type; 
		//OR
		using type = T;
	};

	//partial/complete specialization for const type
	template<typename T> 
	struct MM_remove_const<const T>
	{ 
		//typedef T type;
		//OR
		using type = T;
	};

	//define an alias template for this type_trait
	template<typename T>
	using MM_remove_const_t = typename MM_remove_const<T>::type;

	void test_MM_remove_const()
	{
		static_assert(MM_is_same_v<MM_remove_const<int>::type, int> == true, "");
		static_assert(MM_is_same_v<MM_remove_const<const int>::type, int> == true, "");
		static_assert(MM_is_same<MM_remove_const_t<int>, int>::value == true, "");
		static_assert(MM_is_same_v<MM_remove_const_t<int>, int> == true, "");
	}
	
	//------------------------------------------------------------------

	template<typename... Ts> 
	struct MM_make_void 
	{ 
		//typedef void type;
		using type = void;
	};
	template<typename... Ts> 
	using MM_void_t = typename MM_make_void<Ts...>::type;

	//This metafunction is used in template metaprogramming to detect ill-formed types in SFINAE context
	// primary template handles types that have no nested ::type member:
	template<typename, typename = MM_void_t<>>
	struct has_type_member : std::false_type
	{ };

	// specialization recognizes types that do have a nested ::type member:
	template<typename T>
	struct has_type_member<T, MM_void_t<typename T::type>> : std::true_type
	{ };

	//It can also be used to detect validity of an expression:
	// primary template handles types that do not support pre-increment:
	template<typename, typename = MM_void_t<>>
	struct has_pre_increment_member : std::false_type
	{ };
	// specialization recognizes types that do support pre-increment:
	template<typename T>
	struct has_pre_increment_member<T, MM_void_t<decltype(++std::declval<T&>())>> : std::true_type
	{ };

	void test_MM_make_void_1()
	{
		static_assert(has_type_member<std::remove_const<int>>::value == true, "");
		static_assert(has_type_member<int>::value == false, "");

		static_assert(has_pre_increment_member<std::vector<int>::iterator>::value == true, "");
		static_assert(has_pre_increment_member<void>::value == false, "");
	}

	//Another use: to check if a type is iteratable:
	class A {};

	template<typename T, typename = void>
	struct MM_is_iterable : std::false_type
	{ };
	template<typename T>
	struct MM_is_iterable<
			T, 
			MM_void_t<
				decltype(std::declval<T>().begin()), 
				decltype(++(std::declval<T>().begin())), 
				decltype(std::declval<T>().end())
			>
		>
		: std::true_type 
	{ };

	// An iterator trait which value_type is always the value_type of the 
	// iterated container, even with back_insert_iterator which value_type is void
	template<typename T, typename = void>
	struct MM_iterator_trait : std::iterator_traits<T> 
	{ };
	template<typename T>
	struct MM_iterator_trait<T, MM_void_t<typename T::container_type>>
		: std::iterator_traits<typename T::container_type::iterator> 
	{ };

	void test_MM_make_void_2()
	{
		static_assert(MM_is_iterable<std::vector<double>>::value == true, "");
		static_assert(MM_is_iterable<std::map<int, double>>::value == true, "");
		static_assert(MM_is_iterable<double>::value == false, "");
		static_assert(MM_is_iterable<A>::value == false, "");
		
		std::vector<int> v;
		static_assert(std::is_same<
			MM_iterator_trait<decltype(std::back_inserter(v))>::value_type,
			MM_iterator_trait<decltype(v.cbegin())>::value_type
		>::value == true, "");
	}

	//------------------------------------------------------------------

	template <typename Base>
	static constexpr std::true_type checkIfUpcastingImplicit(const volatile Base*);
	template <typename Base>
	static constexpr std::false_type checkIfUpcastingImplicit(const volatile void*);

	template <typename Base, typename Derived>
	using MM_is_base_of_decltype = decltype(checkIfUpcastingImplicit<Base>(std::declval<Derived*>()));

	template <typename Base, typename Derived, typename = void>
	struct MM_is_base_of_helper : public std::true_type
	{ };

	template <typename Base, typename Derived>
	struct MM_is_base_of_helper<Base, Derived, std::void_t<MM_is_base_of_decltype<Base, Derived>>> : public MM_is_base_of_decltype<Base, Derived>
	{ };

	template<typename B, typename D>
	struct MM_is_base_of : MM_is_base_of_helper<B, D>
	{
	};

	//define constexpr global const object for value
	template<typename B, typename D>
	constexpr const bool MM_is_base_of_v = MM_is_base_of<B, D>::value;

	void test_MM_is_base_of()
	{
		class Base
		{
		};

		class PublicDerived : public Base
		{
		};

		class ProtectedDerived : protected Base
		{
		};

		class D1 : public Base
		{
		};
		class D2 : public Base
		{
		};
		class DD : public D1, public D2
		{
		};

		class PrivateDerived : private Base
		{
		};

		class MultiDerived : public PrivateDerived, public ProtectedDerived
		{
		};

		static_assert(MM_is_base_of<int, Base>::value == false, "");
		static_assert(MM_is_base_of_v<int, Base> == false, "");
		static_assert(MM_is_base_of<Base, PublicDerived>::value == true, "");
		static_assert(MM_is_base_of<Base, ProtectedDerived>::value == true, "");
		static_assert(MM_is_base_of<Base, PrivateDerived>::value == true, "");
		static_assert(MM_is_base_of<Base, DD>::value == true, ""); //Base is ambiguos base class of DD
		static_assert(MM_is_base_of<ProtectedDerived, MultiDerived>::value == true, "");
		static_assert(MM_is_base_of<Base, MultiDerived>::value == true, ""); //Base is ambiguos base class of MultiDerived
	}

	//------------------------------------------------------------------

	// Implementation 1
	/*
	template<typename toT>
	constexpr bool MM_is_convertible_helper_1(toT) { return true; }

	//template<typename toT>
	//std::true_type MM_is_convertible_helper_1(...);

	template<typename fromT, typename toT, typename = MM_void_t<decltype(MM_is_convertible_helper_1<toT>(std::declval<fromT>()))>>
	struct MM_is_convertible_1 : std::true_type
	{
	};

	template<typename fromT, typename toT, typename = MM_void_t<>>
	struct MM_is_convertible_1 : std::false_type
	{
	};

	template<typename fromT, typename toT>
	using MM_is_convertible_1_v = MM_is_convertible_1<fromT, toT>::value;

	void test_MM_is_convertible_1()
	{
		static_assert(MM_is_convertible_1_v<int, float> == true, "");
		static_assert(MM_is_convertible_1_v<int, long> == true, "");
		static_assert(MM_is_convertible_1_v<int, int*> == false, "");
		static_assert(MM_is_convertible_1_v<float, double> == true, "");
		static_assert(MM_is_convertible_1_v<int&, int> == true, "");
		static_assert(MM_is_convertible_1_v<int, int&> == true, "");
	}
	*/

	// Implementation 2 - gcc like implementation

	//template<typename fromT, typename toT>
	//struct MM_is_convertible_2
	//{
	//	template<typename toT1>
	//	constexpr static void check_helper(toT1);

	//	template<typename fromT1, typename toT1, typename = decltype(check_helper<toT1>(std::declval<fromT1>()))>
	//	constexpr static std::true_type check(int);

	//	template<typename, typename>
	//	constexpr static std::false_type check(...);

	//	//typedef decltype(check<fromT, toT>(0)) type;
	//	using value = check<fromT, toT>(0)::value;
	//};

	////template<typename fromT, typename toT>
	////struct MM_is_convertible_2 : public MM_is_convertible_2_helper<fromT, toT>::type
	////{
	////};

	//template<typename fromT, typename toT>
	//using MM_is_convertible_2_v = MM_is_convertible_2<fromT, toT>::value;

	//template<typename _From, typename _To>
	//class __is_convertible_helper<_From, _To>
	//{
	//	template<typename _To1>
	//	static void __test_aux(_To1) {};

	//	template<typename _From1, typename _To1,
	//		typename = decltype(__test_aux<_To1>(std::declval<_From1>()))>
	//		static true_type
	//		__test(int);

	//	template<typename, typename>
	//	static false_type
	//		__test(...);

	//public:
	//	typedef decltype(__test<_From, _To>(0)) type;
	//};


	///// is_convertible
	//template<typename _From, typename _To>
	//struct is_convertible
	//	: public __is_convertible_helper<_From, _To>::type
	//{ };

	//template<typename fromT, typename toT>
	//using MM_is_convertible_2_v = is_convertible<fromT, toT>::value;

	//void test_MM_is_convertible_2()
	//{
	//	static_assert(MM_is_convertible_2_v<int, float> == true, "");
	//	static_assert(MM_is_convertible_2_v<int, long> == true, "");
	//	static_assert(MM_is_convertible_2_v<int, int*> == false, "");
	//	static_assert(MM_is_convertible_2_v<float, double> == true, "");
	//	static_assert(MM_is_convertible_2_v<int&, int> == true, "");
	//	static_assert(MM_is_convertible_2_v<int, int&> == true, "");
	//}

	//------------------------------------------------------------------

	//template<typename Condition, typename ResultIfTrue, typename ResultIfFalse>
	//struct MM_conditional
	//{
	//	using type = ResultIfFalse;
	//};

	//template<typename ResultIfTrue, typename ResultIfFalse>
	//struct MM_conditional<true, ResultIfTrue, ResultIfFalse>
	//{
	//	using type = ResultIfTrue;
	//};

	//template<typename Condition, typename ResultIfTrue, typename ResultIfFalse>
	//using MM_conditional_t = MM_conditional<Condition, ResultIfTrue, ResultIfFalse>::type;

	//------------------------------------------------------------------

	//------------------------------------------------------------------

	//------------------------------------------------------------------
}