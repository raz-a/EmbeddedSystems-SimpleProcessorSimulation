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

	std::ofstream * output = new std::ofstream("simulation.txt");

	Processor * p = new Processor(imem, reg, dmem);
	int i = 0;

	while (!p->IsSteadyState())
	{
		*output << "Step " << i++ << ":" << endl << *p << endl << endl;
		p->NextClock();
	}

	// Last Step
	*output << "Step " << i << ":" << endl << *p << endl << endl;

	output->close();

	delete imem;
	delete reg;
	delete dmem;
	delete p;
	delete output;
}
