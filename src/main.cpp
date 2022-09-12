#include "pch.h"

#include "app.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
	aito::Logger::init();


	aito::Application app{ };

	try
	{
		app.run();
	} catch (const std::exception& e)
	{
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

	return 0;
}
