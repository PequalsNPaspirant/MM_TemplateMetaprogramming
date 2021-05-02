//Goal
//Create a task container and executor identified by string/enum

#include <iostream>
#include <typeinfo>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>
#include <type_traits>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace constexprBitsArray_v1 {

		template<typename T, size_t bits_, std::enable_if_t<std::is_unsigned<T>::value>* dummy = nullptr>
		class constexprBitsArray
		{
		public:
			using type = T;

			constexpr constexprBitsArray(bool flag = false)
				: data_{ 0 }
			{
				data_[numElements_ - 1] = (flag ? ~((T{1} << (bitsInOneElement_ - bitsInLastElement_)) - T{1}) : T{0});
				for(size_t i = 0; i < numElements_ - 1; ++i)
					data_[i] = (flag ? ~(T{0}) : T{0});
			}
			
			//constexpr constexprBitsArray(bool flag = false)
			//	: data_{ 0 }
			//{
			//	setValueAtIndex<numElements_ - 1>(flag, (flag ? ~((T{1} << (bitsInOneElement_ - bitsInLastElement_)) - T{1}) : T{0}));
			//}

			constexpr size_t size() const
			{
				return bits_;
			}

			void setBit(size_t index, bool value)
			{
				size_t arrayIndex = index / constexprBitsArray<T, bits_>::bitsInOneElement_;
				size_t bitIndex = index % constexprBitsArray<T, bits_>::bitsInOneElement_;
				if (value)
					data_[arrayIndex] |= (1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
				else
					data_[arrayIndex] &= ~(1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
			}

			constexpr bool getBit(size_t index) const
			{
				//can not throw in constexpr function
				//if(index >= bits_)
				//	throw std::range_error{ "Index " + std::to_string(index) + " should be less than bits_ " + std::to_string(bits_) };
					
				size_t arrayIndex = index / constexprBitsArray<T, bits_>::bitsInOneElement_;
				size_t bitIndex = index % constexprBitsArray<T, bits_>::bitsInOneElement_;
				return data_[arrayIndex] & (1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
			}

			bool toggleBit(size_t index)
			{
				size_t arrayIndex = index / constexprBitsArray<T, bits_>::bitsInOneElement_;
				size_t bitIndex = index % constexprBitsArray<T, bits_>::bitsInOneElement_;
				data_[arrayIndex] ^= (1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
				//return new value of bit
				return getBit(index);
			}
			
			template<size_t index>
			constexpr T getAt() const
			{
				return data_[index];
			}

		private:
			constexpr static const size_t bytesInOneElement_ = sizeof(T);
			constexpr static const size_t bitsInOneElement_ = bytesInOneElement_ * 8ULL;
			constexpr static const size_t bitsInLastElement_ = (bits_ % bitsInOneElement_) != 0 ? (bits_ % bitsInOneElement_) : bitsInOneElement_;
			constexpr static const size_t numElements_ = (bits_ / bitsInOneElement_) + (bitsInLastElement_ > 0ULL ? 1ULL : 0ULL);

			T data_[numElements_];

			//template<size_t index, std::enable_if_t<index != 0>* dummyT = nullptr>
			//constexpr void setValueAtIndex(bool flag, T value)
			//{
			//	data_[index] = value;
			//	setValueAtIndex<index - 1>(flag, flag ? ~(T{0}) : T{0});
			//}
			//
			//template<size_t index, std::enable_if_t<index == 0>* dummyT = nullptr>
			//constexpr void setValueAtIndex(bool flag, T value)
			//{
			//	data_[0] = value;
			//}

		};

	}

}

