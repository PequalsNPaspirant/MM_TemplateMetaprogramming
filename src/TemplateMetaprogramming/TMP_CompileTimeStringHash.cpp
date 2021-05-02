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
#include "TMP_CompileTimeStringHash.h"

namespace mm {

	namespace compileTimeStringHash_v1 {

		enum Rainbow
		{
			red,
			orange,
			yellow,
			green,
			blue,
			indigo,
			violet
		};

		Rainbow fromString(const std::string& color)
		{
			using namespace compileTimeStringHash_v1;

			size_t h = "red"_hash;

			constexpr size_t hashCodes[] = {
				"red"_hash,
				"orange"_hash,
				"yellow"_hash,
				"green"_hash,
				"blue"_hash,
				"indigo"_hash,
				"violet"_hash
			};

			constexpr const size_t sz = sizeof(hashCodes) / sizeof(size_t);
			constexpr bool hasDup = hasDuplicates(hashCodes, sz);
			static_assert(!hasDup, "array has duplictes");

			const size_t inputHashCode = fnv1a_32(color.c_str(), color.size());
			Rainbow retVal;
			switch (inputHashCode)
			{
			case hashCodes[0]: retVal = Rainbow::red; break;
			case hashCodes[1]: retVal = Rainbow::orange; break;
			case hashCodes[2]: retVal = Rainbow::yellow; break;
			case hashCodes[3]: retVal = Rainbow::green; break;
			case hashCodes[4]: retVal = Rainbow::blue; break;
			case hashCodes[5]: retVal = Rainbow::indigo; break;
			case hashCodes[6]: retVal = Rainbow::violet; break;
			default: throw std::runtime_error{ "Color " + color + " does not exist in Rainbow" };
			}

			return retVal;
		}

		std::string toString(Rainbow color)
		{
			std::string retVal;
			switch (color)
			{
			case Rainbow::red: retVal = "red"; break;
			case Rainbow::orange: retVal = "orange"; break;
			case Rainbow::yellow: retVal = "yellow"; break;
			case Rainbow::green: retVal = "green"; break;
			case Rainbow::blue: retVal = "blue"; break;
			case Rainbow::indigo: retVal = "indigo"; break;
			case Rainbow::violet: retVal = "violet"; break;
			default: throw std::runtime_error{ "Color with index " + std::to_string(static_cast<int>(color)) + " does not exist in Rainbow" };
			}

			return retVal;
		}

		void test()
		{
			MM_EXPECT_TRUE(Rainbow::red == fromString("red"));
			MM_EXPECT_TRUE(Rainbow::orange == fromString("orange"));
			MM_EXPECT_TRUE(Rainbow::yellow == fromString("yellow"));
			MM_EXPECT_TRUE(Rainbow::green == fromString("green"));
			MM_EXPECT_TRUE(Rainbow::blue == fromString("blue"));
			MM_EXPECT_TRUE(Rainbow::indigo == fromString("indigo"));
			MM_EXPECT_TRUE(Rainbow::violet == fromString("violet"));
			MM_EXPECT_TRUE(Rainbow::violet != fromString("red"));
			//MM_EXPECT_TRUE(Rainbow::violet != fromString("purple"));  throws...
			MM_EXPECT_TRUE("red" == toString(Rainbow::red));
			MM_EXPECT_TRUE("orange" == toString(Rainbow::orange));
			MM_EXPECT_TRUE("yellow" == toString(Rainbow::yellow));
			MM_EXPECT_TRUE("green" == toString(Rainbow::green));
			MM_EXPECT_TRUE("blue" == toString(Rainbow::blue));
			MM_EXPECT_TRUE("indigo" == toString(Rainbow::indigo));
			MM_EXPECT_TRUE("violet" == toString(Rainbow::violet));
			MM_EXPECT_TRUE("red" != toString(Rainbow::violet));
			MM_EXPECT_TRUE("purple" != toString(Rainbow::violet));
		}
		
	}

	MM_DECLARE_FLAG(compileTimeStringHash_v1);

	MM_UNIT_TEST(compileTimeStringHash_v1_test1, compileTimeStringHash_v1)
	{
		compileTimeStringHash_v1::test();
	}
}

