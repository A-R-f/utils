/*
This is a simple demo how to use PageWriter.
Use as you wish, at your own risk.
*/

#include <PagedMemory.hpp>
#include <iostream>

//using namespace std;

typedef unsigned int mem_t;

const unsigned int PAGES = 4;
const unsigned int PAGE_SIZE = 8;

mem_t mem[PAGES * PAGE_SIZE];

mem_t src_a[] = {1, 2, 3, 4};
mem_t src_b[] = {5, 6, 7};
mem_t src_c[] = {8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

mem_t* dst = mem;
const mem_t* src = NULL;
unsigned int len = 0;

static void header(const char c)
{
	const unsigned int offset = dst - mem;
	std::cout << "write " << c << ", offset " << offset << ", length " << len << '\n';
}

static void display()
{
	for( unsigned int i = 0 ; i < PAGES ; ++i )
	{
		std::cout << "page " << i << ':';
		for( unsigned int j = 0 ; j < PAGE_SIZE ; ++j )
		{
			std::cout << ' ' << mem[i * PAGE_SIZE + j];
		}
		std::cout << '\n';
	}
}

int main()
{
	typedef PagedMemory<mem_t, mem, PAGES, PAGE_SIZE>::DefaultWriter pw;

	std::cout << "start\n";
	display();

	dst = mem;
	src = src_a;
	len = sizeof(src_a) / sizeof(mem_t);
	header('a');
	pw::write(dst, src, len);
	display();

	dst = mem + 14;
	src = src_b;
	len = sizeof(src_b) / sizeof(mem_t);
	header('b');
	pw::write(dst, src, len);
	display();

	dst = mem + 32 - 12;
	src = src_c;
	len = sizeof(src_c) / sizeof(mem_t);
	header('c');
	pw::write(dst, src, len);
	display();

	return 0;
}
