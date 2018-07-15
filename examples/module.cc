/*
This is a simple demo how to use Module.
Use as you wish, at your own risk.
*/

#ifdef APP	//code for application using the shared object

#include <iostream>
#include <linux/Module.hpp>

int main()
{
	std::cout << "start\n";

	int i = 917;
	std::cout << "i = " << i << '\n';
	Module m("./module.so", Module::Constructor<int>("mod_ctor", i), Module::Destructor("mod_dtor"));
	std::cout << "m.ok(): " << (m.ok() ? "yes" : "no") << '\n';
	std::cout << "i = " << i << '\n';
	const char** stext = m.symbol<const char*>("MODULE_STRING");
	std::cout << "stext @ " << (void*)stext << '\n';
	std::cout << "stext: " << (( stext != NULL ) ? *stext : "NULL") << '\n';
	const int* sint = m.symbol<int>("MODULE_INT");
	std::cout << "sint @ " << (void*)sint << '\n';
	std::cout << "sint: " << (( sint != NULL ) ? *sint : 0) << '\n';
	typedef int(ifun)(void);
	ifun* const fun = m.symbol<ifun>("mod_fun");
	std::cout << "fun @ " << (void*)fun << '\n';
	std::cout << (( fun != NULL ) ? fun() : -1) << std::endl;

	Module m2("./module.so", Module::Constructor<const int>("mod_ctor", 190), Module::Destructor("mod_dtor"));

	return 0;
}

#endif	//APP


#ifdef MOD	//code for shared object

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

extern const char* const MODULE_STRING;
int MODULE_INT = -94;

void mod_ctor(int& arg)
{
	std::cout << "module reporting, arg = " << arg << "\n";
	MODULE_INT = arg++;
}

void mod_dtor(void)
{
	std::cout << "module dismissed\n";
}

int mod_fun(void)
{
	std::cout << "this is an internal function of the module, it returns an int\n";
	return 58;
}

#ifdef __cplusplus
}	//extern "C"
#endif

const char* const MODULE_STRING = "this is a string in the module";

#endif	//MOD
