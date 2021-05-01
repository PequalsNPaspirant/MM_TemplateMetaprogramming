//Goal: Exploit Template Meta Programming - the most powerful feature of C++

//Problem: Compute GCD of two numbers compile time:

#include <cstdint>

//STL way -------------------

#ifdef _MSC_VER

namespace mm {

	namespace STL_Like {

		// CLASS TEMPLATE _Gcd
		template<intmax_t _Ax,
			intmax_t _Bx>
			struct _GcdX
		{	// computes greatest common divisor of _Ax and _Bx
			static const intmax_t value = _GcdX<_Bx, _Ax % _Bx>::value;
		};

		template<intmax_t _Ax>
		struct _GcdX<_Ax, 0>
		{	// computes greatest common divisor of _Ax and 0
			static const intmax_t value = _Ax;
		};

		template<intmax_t _Ax,
			intmax_t _Bx>
			struct _Gcd
		{	// computes greatest common divisor of abs(_Ax) and abs(_Bx)
			static const intmax_t value =
				_GcdX<_Abs<_Ax>::value, _Abs<_Bx>::value>::value;
		};

		template<>
		struct _Gcd<0, 0>
		{	// avoids division by 0 in ratio_less
			static const intmax_t value = 1;	// contrary to mathematical convention
		};

	}

	//More efficient than STL

	template<intmax_t _Ax,
		intmax_t _Bx>
		struct _GcdX
	{	// computes greatest common divisor of _Ax and _Bx
		static const intmax_t value = _GcdX<std::min(_Ax, _Bx), std::max(_Ax, _Bx) % std::min(_Ax, _Bx)>::value;
	};

	template<intmax_t _Ax>
	struct _GcdX<_Ax, 0>
	{	// computes greatest common divisor of _Ax and 0
		static const intmax_t value = _Ax;
	};

	template<intmax_t _Ax,
		intmax_t _Bx>
		struct _Gcd
	{	// computes greatest common divisor of abs(_Ax) and abs(_Bx)
		static const intmax_t value =
			_GcdX<_Abs<_Ax>::value, _Abs<_Bx>::value>::value;
	};

	template<>
	struct _Gcd<0, 0>
	{	// avoids division by 0 in ratio_less
		static const intmax_t value = 1;	// contrary to mathematical convention
	};


	//Gabriel's question:
	//Write a template class that can calculate the powers of 2 compile time

	//very interesting...
	//http://stackoverflow.com/questions/27270541/is-there-no-built-in-way-to-compute-a-power-at-compile-time-in-c

}

#endif