#pragma once

#include "shaderprogram.h"

namespace cppvk {
	class App {
		friend init;
	public:

		const int WIDTH = 800;
		const int HEIGHT = 600;

		void run();

	private:
		GLFWwindow* window;
		VkInstance instance;
		VkDebugReportCallbackEXT callback;

		void initialize();
		void mainloop();
		void clean();
	};
}