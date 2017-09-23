#include "app.h"

void cppvk::App::run()
{
	initialize();
	mainloop();
	clean();
}

void cppvk::App::initialize()
{
	cppvk::init::glfw(this);

	cppvk::init::vkinstance(this);

	cppvk::init::vkdebugCallback(this);
}

void cppvk::App::mainloop()
{
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
	}
}

void cppvk::App::clean()
{
	cppvk::destroyDebugReportCallbackEXT(instance, callback, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}
