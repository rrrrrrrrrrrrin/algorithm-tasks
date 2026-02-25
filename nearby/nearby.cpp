#include <fstream>
#include <iostream>
#include "stdint.h"
#include "vector.h"

uint64_t binary_search()
{
	uint64_t res = 0;
	return 0;
}

int main(int argc, char* argv[]) 
{
	if (argc != 3)
	{
		std::cout << "Usage: input-file output-file";
		return 1;
	}

	std::ifstream input(argv[1]);
	if (!input)
	{
		std::cout << "Couldn't open input file";
		return 2;
	}

	std::ofstream output(argv[2]);
	if (!output)
	{
		std::cout << "Couldn't open output file";
		return 3;
	}

	int n;
	int k;
	input >> n;
	input >> k;
	input.ignore();

	uint64_t* vec = new uint64_t[n];

	size_t i = 0;
	char ch;
	while ((ch = input.get()) != EOF)
	{
		if (ch == ' ') { continue; }
		if (ch == '\n') { break; }
		vec[i++] = static_cast<uint64_t>(ch - '0');
	}

	while ((ch = input.get()) != EOF)
	{
		if (ch == ' ') { continue; }

	}

	delete vec;
	input.close();

	return 0;
}