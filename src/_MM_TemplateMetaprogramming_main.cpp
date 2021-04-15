// Test.cpp : Defines the entry point for the console application.

#include <iostream>

//#include "WindowsUtils/WindowsUtils.h"
//#include "Timer/Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
using namespace mm;

namespace mm {

	MM_DEFINE_FLAG(false, TemplateMetaProgrammingBasicSyntax);
	MM_DEFINE_FLAG(false, TemplateMetaProgramming_tuple);
	MM_DEFINE_FLAG(false, TMP_unique_ptr_polymorphism);
	MM_DEFINE_FLAG(false, MM_Any_type);
	MM_DEFINE_FLAG(false, MM_returnTypeCovariance);
	MM_DEFINE_FLAG(false, TemplateMetaProgrammingCustomTaskExecutor_v1);
	MM_DEFINE_FLAG(true, TemplateMetaProgrammingCustomTaskExecutor_v5);
}

int main(int argc, char* argv[])
{
	//MaximizeWindow();				

	MM_RUN_UNIT_TESTS;

	//cout << "\n\n\n" << Timer::getCurrentLocalTimeInNanoSeconds() << " CONGRATULATIONS!!! End of program reached successfully.\n\n\n";
	cout << "\n\n\n" << " CONGRATULATIONS!!! End of program reached successfully.\n\n\n";

	std::cin.get();
	return 0;
}

