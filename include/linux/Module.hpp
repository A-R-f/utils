/*
Module.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_MODULE_HPP
#define __LINUX_MODULE_HPP

#include <cstdlib>
#include <dlfcn.h>
#include <string>


struct Module {

	template< typename T >
	class Constructor {

		const std::string& name;
		T& arg;

	public:

		typedef void(Type)(T&);

		Constructor(const std::string& s, T& t) : name(s), arg(t) {}

		void operator()(const Module& m) const
		{
			const Type* const ctor = m.symbol<Type>(name);
			if( ctor != NULL ) { ctor(arg); }
		}

	};


	class Destructor {
		const std::string& name;
	public:
		typedef void(Type)(void);
		Destructor(const std::string& s = std::string()) : name(s) {}
		const Type* ptr(const Module& m) const { return m.symbol<Type>(name); }
	};

private:

	void* hdl;
	const Destructor::Type* destructor;

	bool __init(const char* const path, const int flag)
	{
		destructor = NULL;
		hdl = dlopen(path, flag);
		return hdl != NULL;
	}

	template< typename T >
	void __init(const char* const path, const Constructor<T>& ctor, const Destructor& dtor, const int flag)
	{
		if( !__init(path, flag) ) { return; }
		destructor = dtor.ptr(*this);
		ctor(*this);
	}

public:

	Module(const char* const path, const int flag = RTLD_NOW) { __init(path, flag); }

	template< typename T >
	Module(const char* const path, const Constructor<T>& ctor, const Destructor& dtor = Destructor(), const int flag = RTLD_NOW) { __init(path, ctor, dtor, flag); }

	~Module() { if( destructor != NULL ) { destructor(); } }

	bool ok() const { return hdl != NULL; }

	template< typename T >
	T* symbol(const std::string& name) const { return (T*)dlsym(hdl, name.c_str()); }

};


template<>
class Module::Constructor<void> {

	const std::string& name;

public:

	typedef void(Type)(void);

	Constructor(const std::string& s) : name(s) {}

	void operator()(const Module& m) const
	{
		const Type* const ctor = m.symbol<Type>(name);
		if( ctor != NULL ) { ctor(); }
	}

};


#endif
