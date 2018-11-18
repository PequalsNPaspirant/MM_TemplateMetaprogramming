

/*

//TODO: resolve this question asked on: 
https://stackoverflow.com/questions/8108877/c11-calculating-variadic-function-parameter-types?rq=1

#include <iostream>

template<typename T, typename Arg>
void pass_through_1(Arg arg)
{
	T::f(arg);
}

template<typename T> struct arg_of_1;

template<typename Ret, typename Arg>
struct arg_of_1<Ret(Arg)>
{
	typedef Arg type;
};

template<typename T>
void up_front_1(typename arg_of_1<decltype(T::f)>::type arg)
{
	T::f(arg);
}

template<typename T, typename... Args>
void pass_through_var(Args... args)
{
	T::f(args...);
}

template<typename T> struct args_of_var;

template<typename...> struct type_list;

template<typename Ret, typename... Args>
struct args_of_var<Ret(Args...)>
{
	// typedef Args... type; // can't do this
	typedef type_list<Args...> type;
};

// template<typename T>
// void up_front_var(typename args_of_var<decltype(T::f)>::type... args) // can't do this
// {
//     T::f(args...);
// }

struct test
{
	static void f(int x) { std::cout << x * 9 << std::endl; }
};

int test(int, char**)
{
	pass_through_1<test>(7);
	up_front_1<test>(8);
	pass_through_var<test>(9);
	// up_front_var<test>(10);
	return 0;
}

/*

/*

https://stackoverflow.com/questions/24948277/unpacking-arguments-of-a-functional-parameter-to-a-c-template-class?rq=1

Question:

I have a question involving functional template arguments to template classes in C++.

I'd like to define a template class Foo taking a single template parameter Fun

template <typename Fun>
struct Foo {
...
};
such that given a function like

void bar(std::string a, float b, char c)
{
...
}
then Foo<bar>::args_t will be equivalent to a typedef for

std::tuple<std::string, float, char>
Is this possible? (The use of std::tuple here is just for concreteness. More generally I'm wondering if it's possible to do something like pattern-matching on the arguments of a functional template parameter.)

The point is to avoid having to define Foo in a way like

template Foo<typename A, typename B, typename C, typename D,
D (*Fun)(A a, B b, C c)>
struct Foo {
typedef std::tuple<A,B,C>  args_t;
};
which requires both committing to a fixed number of function arguments, and requiring the argument and return types of the function to be provided explicitly as template parameters. (Defining Foo using variadic templates could presumably solve the former issue, but what about the latter?)

Thanks!

Answer:
Declare a primary template and leave it unimplemented.

template<typename T>
struct foo;     // unimplemented primary template
Then provide a partial specialization that matches function types as the template argument.

template<typename Result, typename... Args>
struct foo<Result(Args...)>
{
using args_t = std::tuple<Args...>;
};
You can access the nested type as

foo<decltype(bar)>::args_t

*/