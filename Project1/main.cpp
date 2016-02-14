/*
	Testing environment
*/

#include <iostream>
#include <string>
#include <fstream>
#include "Processor.h"

using namespace Project1;

void main()
{
	std::ifstream * imem = new std::ifstream("instructions.txt");
	std::ifstream * reg = new std::ifstream("registers.txt");
	std::ifstream * dmem = new std::ifstream("datamemory.txt");

	Processor * p = new Processor(imem, reg, dmem);

	int x = 7;

	delete imem;
	delete reg;
	delete dmem;
	delete p;
}
