/*
// c plus plus vulkan sample
// file: cpp-vulkan.cpp
//     description: main file
// github: https://github.com/patrickhadlaw/py-mod-sort
// by: Patrick Stanislaw Hadlaw
*/

#include "app.h"

int main(){

	cppvk::App app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}