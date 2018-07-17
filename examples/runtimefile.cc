/*
This is a simple demo how to use Pipe.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <unistd.h>

#include <RunTimeFile.hpp>
#include <linux/PIDFile.hpp>

int main(int argc, char** argv)
{
	if( argc < 2 )
	{
		std::cout << "Usage:\n"
		          << '\t' << argv[0] << " <pidfile1> [pidfile2]" << std::endl;
		return 0;
	}

	RunTimeFile rtf(argv[1], getpid());
	PIDFile* pidfile = NULL;

	if( argc > 2 ) { pidfile = new PIDFile(argv[2]); }

	if( rtf.created() )
	{
		std::cout << "You can now check PID in " << rtf.path() << ", ";
		if( pidfile != NULL ) { std::cout << "and in " << pidfile->path() << ", "; }
		std::cout << "and then press ENTER." << std::endl;
		std::cin.get();
	}
	else
	{
		std::cout << "unable to create " << rtf.path() << std::endl;
	}

	delete pidfile;

	return 0;
}
