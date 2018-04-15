/*
	cpp-vulkan runs the vulkan app
*/

#include "app.h"

int main(){

	std::cout << "cpp-vulkan VERSION: " << CPPVK_VERSION_MAJOR << '.' << CPPVK_VERSION_MINOR << '.' << CPPVK_VERSION_REVISION << std::endl;

	cppvk::App app;

	try {
		app.initialize();

		while (!app.shouldClose()) {
			app.mainLoop();
		}
		
		app.deviceWaitIdle();

		app.clean();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cin.get();
		return -1;
	}
	return 0;
}