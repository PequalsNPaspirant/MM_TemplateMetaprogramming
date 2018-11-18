//Goal: Exploit Template Meta Programming - the most powerful feature of C++

//Problem: Compute GCD of two numbers compile time:

#include <cstdint>
using namespace std;

#include <TemplateMetaprogramming/TemplateMetaProgramming_basics.h>
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

/*
References:
https://eli.thegreenplace.net/2014/variadic-templates-in-c/
http://lbrandy.com/blog/2013/03/variadic_templates/
http://en.cppreference.com/w/cpp/language/parameter_pack
http://www.cplusplus.com/articles/EhvU7k9E/ (Note: many sources at the end of page)
cast.com/2015/08/edouard-alligand/

Finally a very interesting article:
if recursive use of variadic templates reminds you of pattern matching in functional languages, you're right!
Technically, this is not recursion, because a different function is called. The compiler ends up generating 
a different function for every used length of the parameter pack. It's useful to reason about it recursively, though.
More at...
https://bartoszmilewski.com/2009/10/21/what-does-haskell-have-to-do-with-c/
http://www.osl.iu.edu/~dgregor/cpp/variadic-templates.pdf

Videos:
https://www.youtube.com/watch?v=dD57tJjkumE
https://www.youtube.com/watch?v=u_8MMuVFz_k
https://www.youtube.com/watch?v=K4c8QJvueas

Introduction:

Prior to C++11, the only way to write functions that take an arbitrary number of arguments was to use variadic functions 
like printf, with the ellipsis syntax (...) and the accompanying va_ family of macros. If you've ever written code 
using this approach you know how cumbersome it is. In addition to being type unsafe (all type resolution has to be 
done explicitly with casts in va_arg, at runtime), it's also tricky to get right. The va_ macros perform low-level 
memory manipulation, and I've seen a lot of code that segfaults because it isn't using them carefully enough.
But what always bothered me most with this approach is leaving something that is clearly known at compile-time, 
to run-time. Yes, when we write a variadic function we don't know all the ways it's going to be used. But when 
the compiler puts the whole program together, it does know. It sees perfectly well all the invocations of the 
function throughout the program, and all the possible argument types it gets passed (types are, after all, 
resolved at compile-time in C++).
Finally, there's a way to write functions that take an arbitrary number of arguments in a type-safe way and 
have all the argument handling logic resolved at compile-time, rather than run-time.

--------------------------------------------------------------------------------
Ellipses (...) and Variadic Templates:
A variadic template is a class or function template that supports an arbitrary number of arguments.

A template parameter pack is a template parameter that accepts zero or more template arguments (non-types, types, or templates). 
A function parameter pack is a function parameter that accepts zero or more function arguments.
A template with at least one parameter pack is called a variadic template.

template<typename T, typename... Args>
T adder(T first, Args... args) {
	return first + adder(args...);
}

typename... Args is called a template parameter pack, and Args... args is called a function parameter pack
--------------------------------------------------------------------------------
Syntax:
template<typename... Arguments> class classname;
OR
template<typename ...Arguments> class classname;
OR
template<typename ... Arguments> class classname;

--------------------------------------------------------------------------------
Example 3:
In a primary class template, the template parameter pack must be the final parameter in the template parameter list. 
In a function template, the template parameter pack may appear earlier in the list provided that all following 
parameters can be deduced from the function arguments, or have default arguments:

template<typename... Ts, typename U> struct Invalid; // Error: Ts.. not at the end

template<typename ...Ts, typename U, typename=void>
void valid(U, Ts...);     // OK: can deduce U
// void valid(Ts..., U);  // Can't be used: Ts... is a non-deduced context in this position

valid(1.0, 1, 2, 3);      // OK: deduces U as double, Ts as {int,int,int}

--------------------------------------------------------------------------------


*/

namespace mm {

	MM_DECLARE_FLAG(TemplateMetaProgrammingBasicSyntax)

	template<class ... Types>
	struct Tuple
	{
	};

	MM_UNIT_TEST(TemplateMetaProgrammingBasicSyntaxTest_1, TemplateMetaProgrammingBasicSyntax)
	{
		Tuple<> t0;           // Types contains no arguments
		Tuple<int> t1;        // Types contains one argument: int
		Tuple<int, float> t2; // Types contains two arguments: int and float
		//Tuple<0> error;       // error: 0 is not a type
		MM_EXPECT_TRUE(true)
	}


	template<class ... Types>
	int f(Types ... args)
	{
		return 2735;
	}

	MM_UNIT_TEST(TemplateMetaProgrammingBasicSyntaxTest_2, TemplateMetaProgrammingBasicSyntax)
	{
		MM_EXPECT_TRUE(f() == 2735)       // OK: args contains no arguments
		MM_EXPECT_TRUE(f(1) == 2735)      // OK: args contains one argument: int
		MM_EXPECT_TRUE(f(2, 1.0) == 2735) // OK: args contains two arguments: int and double
	}

	//The template definition gets more and more specialized as you go down...
	template<typename... Types>
	int addAllNumbers(Types... values)  // Most generic template: Handles zero or more arguments
	{
		return 0;
	}
	template<typename FirstType, typename... RemainingTypes>
	FirstType addAllNumbers(FirstType val, RemainingTypes... values) // Little more specialized: Handles one or more arguments
	{
		return val + addAllNumbers(values...);
	}
	template<typename  ExaclyOneType>
	ExaclyOneType addAllNumbers(ExaclyOneType value) //Most specialized: Handles exactly one argument
	{
		return value;
	}
	MM_UNIT_TEST(TemplateMetaProgrammingBasicSyntaxTest_3, TemplateMetaProgrammingBasicSyntax)
	{
		MM_EXPECT_TRUE(addAllNumbers() == 0)
		MM_EXPECT_TRUE(addAllNumbers(1) == 1)
		MM_EXPECT_TRUE(addAllNumbers(5) == 5)
		MM_EXPECT_TRUE(addAllNumbers(1, 2) == 3)
		MM_EXPECT_TRUE(addAllNumbers(1, 2, 3, 4, 5) == 15)
		MM_EXPECT_TRUE(addAllNumbers(1, 'A', 3, 'C', 5) != (1 + 'A' + 3 + 'C' + 5))
	}

	
	template<typename... Types>
	size_t getNumArguments(const Types&... args)
	{
		constexpr auto numargs{ sizeof...(Types) };
		return numargs;
	}

	MM_UNIT_TEST(TemplateMetaProgrammingBasicSyntaxTest_4, TemplateMetaProgrammingBasicSyntax)
	{
		MM_EXPECT_TRUE(getNumArguments() == 0)
		MM_EXPECT_TRUE(getNumArguments(1) == 1)
		MM_EXPECT_TRUE(getNumArguments(5) == 1)
		MM_EXPECT_TRUE(getNumArguments(1, 2) == 2)
		MM_EXPECT_TRUE(getNumArguments(1, 2, 3, 4, 5) == 5)
	}
}