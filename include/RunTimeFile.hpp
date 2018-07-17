#ifndef __RUNTIMEFILE_HPP
#define __RUNTIMEFILE_HPP


#include <cstdio>
#include <fstream>

class RunTimeFile {

	std::string filepath;
	bool filecreated;
	bool fileautoerase;

public:

	template< typename T >
	RunTimeFile(const std::string& _path, const T& t, const bool addnewline = true, const bool _autoerase = true) :
		filepath(_path),
		filecreated(false),
		fileautoerase(_autoerase)
	{
		std::ofstream of(path().c_str());
		if( of )
		{
			of << t;
			if( addnewline ) { of << '\n'; }
			filecreated = true;
		}
	}

	~RunTimeFile() { if( created() && autoerase() ) { std::remove(filepath.c_str()); } }

	bool autoerase() const { return fileautoerase; }
	bool created() const { return filecreated; }
	const std::string& path() const { return filepath; }

};


#endif
