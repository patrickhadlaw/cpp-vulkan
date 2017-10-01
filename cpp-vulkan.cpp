/*
// c plus plus vulkan sample
// file: cpp-vulkan.cpp
//     description: main file
// github: https://github.com/patrickhadlaw/py-mod-sort
// by: Patrick Stanislaw Hadlaw
*/

// app.h: defines cppvk::App class containing intialization, mainloop, cleanup functions as well as containing all vulkan/glfw data
#include "app.h"

int main(){

	cppvk::App app;

	try {
		app.initialize();

		while (!app.shouldClose()) {
			app.mainLoop();
		}

		app.clean();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}