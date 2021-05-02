//Goal
//Create a task container and executor identified by string/enum

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace compileTimeStringHash_v1 {

		template<typename T>
		constexpr bool hasDuplicatesHelper(const T* array, const size_t lhsIndex, const size_t rhsIndex)
		{
			return array[lhsIndex] == array[rhsIndex] ||
				(rhsIndex == 0 ? false : hasDuplicatesHelper(array, lhsIndex, rhsIndex - 1));
		}

		template<typename T>
		constexpr bool hasDuplicates(const T* array, const size_t size)
		{
			return size <= 1 ? false
				: hasDuplicatesHelper(array, size - 1, size - 2) || hasDuplicates(array, size - 1);
		}

		constexpr size_t addWithoutOverflow(const size_t lhs, const size_t rhs)
		{
			return rhs == 0 ? lhs
				: addWithoutOverflow(lhs ^ rhs, (lhs & rhs) << 1);
		}

		constexpr size_t multiplyBy33(const size_t hash)
		{
			return addWithoutOverflow(hash << 5, hash);
		}

		constexpr size_t fnv1a_32(const char* s, size_t size)
		{
			return size == 0 ? 5381ull
				: addWithoutOverflow(multiplyBy33(fnv1a_32(s, size - 1)), s[size - 1]);
		}

		constexpr size_t operator"" _hash(const char* s, size_t size)
		{
			return fnv1a_32(s, size);
		}

	}

}

