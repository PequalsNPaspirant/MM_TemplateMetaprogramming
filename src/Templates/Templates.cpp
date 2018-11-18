//Goal
//All miscellaneous problems

#include <iostream>
#include <typeinfo>
using namespace std;

namespace mm {

	template <typename T>
	void FunTakesValueOrPointer(T input)
	{
		cout << "\nThe type: " << typeid(T).name();
	}

	template <typename T>
	void FunAlwaysTakesPointer(T* input)
	{
		cout << "\nThe pointer of type: " << typeid(T).name();
	}

	void testArgumentTypesToTemplate()
	{
		int n = 10;
		double d = 10.12;
		int* pn = nullptr;
		double* pd = &d;

		FunTakesValueOrPointer(n);
		FunTakesValueOrPointer(d);
		FunTakesValueOrPointer(pn);
		FunTakesValueOrPointer(pd);

		//	FunAlwaysTakesPointer(n); //error C2784: 'void FunAlwaysTakesPointer(T *)': could not deduce template argument for 'T *' from 'int'
		//	FunAlwaysTakesPointer(d); //error C2784: 'void FunAlwaysTakesPointer(T *)': could not deduce template argument for 'T *' from 'double'
		FunAlwaysTakesPointer(pn);
		FunAlwaysTakesPointer(pd);
	}

	//#include <cxxabi.h>

	//template<typename T>
	//std::string type_name()
	//{
	//	int status;
	//	std::string tname = typeid(T).name();
	//	char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status); //Available only in GCC
	//	if (status == 0) {
	//		tname = demangled_name;
	//		std::free(demangled_name);
	//	}
	//	return tname;
	//}
	//
	//class MyExampleClass
	//{
	//
	//};
	//
	//void testGetTypeName()
	//{
	//	cout << "\nType name is: " << type_name<int>;
	//	cout << "\nType name is: " << type_name<double*>;
	//	cout << "\nType name is: " << type_name<MyExampleClass>;
	//	cout << "\nType name is: " << type_name<MyExampleClass**>;
	//}

	void Templates()
	{
		//testArgumentTypesToTemplate();
		//testGetTypeName();
	}

}
