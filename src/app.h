#ifndef AITO_APP_H
#define AITO_APP_H

#include <memory>
#include <vector>

#include "window.h"
#include "renderer.h"
#include "object.h"
#include "descriptor.h"


#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace aito
{
	class Application
	{

	public:
		static constexpr size_t WIDTH = 800;
		static constexpr size_t HEIGHT = 600;

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void run();

		void render_ui();

	private:
		Window window_{ WIDTH, HEIGHT, "3D" };
		Device device_{ window_ };
		Renderer renderer_{ window_, device_ };

		ImGuiContext* imgui_context_;
		ImGuiIO& io_;

		std::unique_ptr<DescriptorPool> globalPool_{};
		std::vector<Object> objects_; // TEMP
		
		void loadObjects(); // TEMP
	};
}



#endif /* AITO_APP_H */
