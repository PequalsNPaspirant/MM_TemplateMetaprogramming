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

			//constexpr constexprBitsArray()
			//	: data_{ 0 }
			//{}
			constexpr constexprBitsArray(bool flag = false)
			{
				//setAllBits(flag);
				data_[numElements_ - 1] = (flag ? ~((T{1} << (bitsInOneElement_ - bitsInLastElement_)) - T{1}) : T{0});
				for(size_t i = 0; i < numElements_ - 1; ++i)
					data_[i] = (flag ? ~(T{0}) : T{0});
			}

			constexpr size_t size() const
			{
				return bits_;
			}

			//constexpr void setAllBits(bool flag)
			//{
			//	setAllBitsHelper<numElements_ - 1>(flag);
			//}

			void setBit(size_t index, bool value)
			{
				size_t arrayIndex = index / constexprBitsArray<T, bits_>::bitsInOneElement_;
				size_t bitIndex = index % constexprBitsArray<T, bits_>::bitsInOneElement_;
				if (value)
					data_[arrayIndex] |= (1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
				else
					data_[arrayIndex] &= ~(1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
			}

			bool getBit(size_t index) const
			{
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
				return data_[arrayIndex] & (1ULL << (constexprBitsArray<T, bits_>::bitsInOneElement_ - bitIndex - 1)); //bit index 0 starts at left
			}
			
			template<size_t index>
			constexpr T getAt()
			{
				return data_[index];
			}

		private:
			constexpr static const size_t bytesInOneElement_ = sizeof(T);
			constexpr static const size_t bitsInOneElement_ = bytesInOneElement_ * 8ULL;
			constexpr static const size_t bitsInLastElement_ = (bits_ % bitsInOneElement_) != 0 ? (bits_ % bitsInOneElement_) : bitsInOneElement_;
			constexpr static const size_t numElements_ = (bits_ / bitsInOneElement_) + (bitsInLastElement_ > 0ULL ? 1ULL : 0ULL);

			T data_[numElements_];

			template<size_t index, std::enable_if_t<index != 0>* dummyT = nullptr>
			constexpr constexprBitsArray<T, bits_>& setAllBitsHelper(bool flag)
			{
				constexpr size_t remainintBits = bits_ % bitsInOneElement_;
				return setValueAtIndex<index>(
					(index != (numElements_ - 1) || remainintBits == 0)
						? std::numeric_limits<T>::max()
						: ~((1ULL << (bitsInOneElement_ - remainintBits)) - 1ULL)
					).setAllBitsHelper<index - 1>(flag);
			}

			template<size_t index, std::enable_if_t<index == 0>* dummyT = nullptr>
			constexpr constexprBitsArray<T, bits_>& setAllBitsHelper(bool flag)
			{
				return setValueAtIndex<0>(flag ? std::numeric_limits<T>::max() : T{0});
			}

			template<size_t index>
			constexpr constexprBitsArray<T, bits_>& setValueAtIndex(T value)
			{
				data_[index] = value;
				return *this;
			}
		};
		
		//template<typename T, size_t bits_, size_t index>
		//constexpr constexprBitsArray<T, bits_>& constexprBitsArray<T, bits_>::setAllBitsHelper(bool flag)
		//{
		//	constexpr size_t remainintBits = bits_ % bitsInOneElement_;
		//	return setValueAtIndex<index>(
		//		index != (numElements_ - 1) || remainintBits == 0
		//			? std::numeric_limits<T>::max()
		//			: ~((1ULL << (bitsInOneElement_ - remainintBits)) - 1ULL)
		//		).setAllBitsHelper<index - 1>(flag);
		//}
		//	
		//template<typename T, size_t bits_>
		//constexpr constexprBitsArray<T, bits_>& constexprBitsArray<T, bits_>::setAllBitsHelper<0>(bool flag)
		//{
		//	return setValueAtIndex<0>(flag);
		//}

	}

}

