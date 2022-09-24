#include "pch.h"

#include "app.h"

#include "keyboardController.h"
#include "camera.h"
#include "simple_render_system.h"
#include "point_light_system.h"
#include "bounding_box_render_system.h"
#include "time.h"

#include "bounds.h"

#include <stdexcept>
#include <array>
#include <iostream>


namespace aito
{
	struct GlobalUbo
	{
		Mat4f projection{ 1.0f };
		Mat4f view{ 1.0f };
		Vec4f ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f };
		Point3f lightPosition{ -0.4f, -1.0f, -1.2f };
		alignas(16) Vec4f lightColor{ 0.29019607843137254901960784313725f, 0.25490196078431372549019607843137f, 0.16470588235294117647058823529412f, 0.7f };
	};

	Application::Application()
		: imgui_context_(ImGui::CreateContext()), io_(ImGui::GetIO())
	{
		globalPool_ = DescriptorPool::Builder(device_)
			.setMaxSets(Swapchain::MAX_FRAMES_IN_FLIGHT + 1)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.build();

		// Initialize ImGui
		ImGui::SetCurrentContext(imgui_context_);

		io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io_.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io_.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io_.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		
		ImGui_ImplGlfw_InitForVulkan(window_.getGLFWWindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		device_.populateImGui_initInfo(init_info);
		renderer_.populateImGui_initInfo(init_info);
		globalPool_->populateImGui_initInfo(init_info);
		ImGui_ImplVulkan_Init(&init_info, renderer_.getSwapChainRenderPass());

		// Upload Fonts
		{
			// Use any command queue
			VkCommandPool command_pool = device_.getCommandPool();
			VkCommandBuffer command_buffer = device_.beginSingleTimeCommands();

			///*if (!vkResetCommandPool(device_.device(), command_pool, 0))
			//{
			//	throw std::runtime_error("Failed to reset command pool");
			//}*/
			//VkCommandBufferBeginInfo begin_info = {};
			//begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			//begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			//if (!vkBeginCommandBuffer(command_buffer, &begin_info))
			//{
			//	throw std::runtime_error("Failed to begin recording command buffer for initializing fonts");
			//}

			ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

			device_.endSingleTimeCommands(command_buffer);
			
			ImGui_ImplVulkan_DestroyFontUploadObjects();

			//globalPool_->resetPool();
		}
		

		// Load objects
		loadObjects();
	}

	Application::~Application()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(imgui_context_);
	}

	void Application::run()
	{
		std::vector<std::unique_ptr<Buffer>> uniformBuffers(Swapchain::MAX_FRAMES_IN_FLIGHT);
		for (auto& bufferPtr : uniformBuffers)
		{
			bufferPtr = std::make_unique<Buffer>(device_,
												 sizeof(GlobalUbo),
												 Swapchain::MAX_FRAMES_IN_FLIGHT,
												 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
												 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
												 device_.properties.limits.minUniformBufferOffsetAlignment);
			bufferPtr->map();
		}

		auto globalSetLayout = DescriptorSetLayout::Builder(device_)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(Swapchain::MAX_FRAMES_IN_FLIGHT);
		for (size_t i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uniformBuffers[i]->descriptorInfo();
			DescriptorWriter(*globalSetLayout, *globalPool_)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ device_, renderer_.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		PointLightSystem pointLightSystem{ device_, renderer_.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		BoundingBoxRenderSystem boundingBoxSystem{ device_, renderer_.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };

		Camera camera{};
		Object viewerObject;
		viewerObject.transform.translation.z = -2.3f;
		viewerObject.transform.translation.y = -0.5f;
		KeyboardController cameraController{};

		Time time;

		float currRot = 0;

		// !!!!!!!!!!!!!!!!!!!! TEMP !!!!!!!!!!!!!!!!!!!!!!!!!!
		Bounds3f boundsa({ -0.5f, 0, -0.5f }, { 0.5f, -0.6f, 0.5f });
		Bounds3f boundsb({ -1.5f, 0, -1.5f }, { -0.7f, -1, -0.7f });

		std::vector<BoundingBoxObject> boundingBoxes;
		/*std::array<BoundingBox::Vertex, 8> vertices = 
		{
			BoundingBox::Vertex({-0.5f, -0.5f, 0.5f }),
			BoundingBox::Vertex({ 0.5f, -0.5f, 0.5f }),
			BoundingBox::Vertex({ -0.5f, 0.5f, 0.5f }),
			BoundingBox::Vertex({0.5f, 0.5f, 0.5f}),
			BoundingBox::Vertex({-0.5f, -0.5f, -0.5f}),
			BoundingBox::Vertex({0.5f, -0.5f, -0.5f}),
			BoundingBox::Vertex({-0.5f, 0.5f, -0.5f}),
			BoundingBox::Vertex({0.5f, 0.5f, -0.5f})
		};*/
		{
			BoundingBoxObject boundingBoxObject;
			boundingBoxObject.model = std::make_shared<BoundingBox>(device_, boundsa);
			boundingBoxObject.transform.translation = { 0.0f, 0.0f, 0.0f };
			boundingBoxObject.transform.scale = { 1.0f, 1.0f, 1.0f };
			boundingBoxObject.transform.rotation = { 0.0f, 0.0f, 0.0f };
			boundingBoxes.push_back(boundingBoxObject);
		}
		{
			BoundingBoxObject boundingBoxObject;
			boundingBoxObject.model = std::make_shared<BoundingBox>(device_, boundsb);
			boundingBoxObject.transform.translation = { 0.0f, 0.0f, 0.0f };
			boundingBoxObject.transform.scale = { 1.0f, 1.0f, 1.0f };
			boundingBoxObject.transform.rotation = { 0.0f, 0.0f, 0.0f };
			boundingBoxes.push_back(boundingBoxObject);
		}
		{
			BoundingBoxObject boundingBoxObject;
			boundingBoxObject.model = std::make_shared<BoundingBox>(device_, bounds_union(boundsa, boundsb), Vec3f{ 0.0f, 0.0f, 1.0f });
			boundingBoxObject.transform.translation = { 0.0f, 0.0f, 0.0f };
			boundingBoxObject.transform.scale = { 1.0f, 1.0f, 1.0f };
			boundingBoxObject.transform.rotation = { 0.0f, 0.0f, 0.0f };
			boundingBoxes.push_back(boundingBoxObject);
		}
		
		// !!!!!!!!!!!!!!!!!!!! ^^^^ !!!!!!!!!!!!!!!!!!!!!!!!!!

		while (!window_.shouldClose())
		{
			glfwPollEvents();

			time.updateTime();

			cameraController.moveInPlaneXZ(window_.getGLFWWindow(), time.deltaTime(), viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = renderer_.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);
			
			// BeginFrame returns a nullptr if the swapchain needs to be recreated. 
			// This skips the frame draw call, if that's the case.
			auto commandBuffer = renderer_.beginFrame();
			if (commandBuffer != nullptr)
			{
				const FrameInfo frameInfo{
					renderer_.getFrameIndex(),
					time.deltaTime(),
					commandBuffer,
					camera,
					globalDescriptorSets[renderer_.getFrameIndex()]
				};



				// Update
				currRot += glm::two_pi<float>() / 3.0f * time.deltaTime();

				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				ubo.lightPosition = { cos(currRot) * 1.3f, -1.0f, sin(currRot) * 1.3f };
				uniformBuffers[frameInfo.frameIndex]->writeToBuffer(&ubo);
				uniformBuffers[frameInfo.frameIndex]->flush();

				


				// Render
				renderer_.beginSwapChainRenderPass(commandBuffer);
				// Render scene
				simpleRenderSystem.renderObjects(frameInfo, objects_);
				pointLightSystem.renderObjects(frameInfo);
				boundingBoxSystem.renderObjects(frameInfo, boundingBoxes);

				// Render GUI
				ImGui_ImplVulkan_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				render_ui();

				// End render
				ImGui::Render();
				auto drawData = ImGui::GetDrawData();
				ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
				// Update and Render additional Platform Windows
				if (io_.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}

				renderer_.endSwapChainRenderPass(commandBuffer);
				renderer_.endFrame();				
			}
		}

		vkDeviceWaitIdle(device_.device());
	}

	void Application::render_ui()
	{

		ImGui::Begin("Info");

		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Frame Time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::SliderFloat2("Smooth Vase X and Y", &objects_[0].transform.translation.x, -5.0f, 5.0f);

		ImGui::End();

		
		
	}


	void Application::loadObjects()
	{
		{
			std::shared_ptr<Model> model = Model::createModelFromFile(device_, "models/smooth_vase.obj");

			Object object;
			object.model = model;
			object.transform.translation = { -0.8f, 0.0f, 0.0f };
			object.transform.scale = Vec3f(3);
			//object.transform.rotation.x = 0.1f * glm::two_pi<float>();

			objects_.push_back(std::move(object));
		}
		{
			std::shared_ptr<Model> model = Model::createModelFromFile(device_, "models/flat_vase.obj");

			Object object;
			object.model = model;
			object.transform.translation = { 0.8f, 0.0f, 0.0f };
			object.transform.scale = Vec3f(3);
			//object.transform.rotation.x = 0.1f * glm::two_pi<float>();

			objects_.push_back(std::move(object));
		}
		{
			std::shared_ptr<Model> model = Model::createModelFromFile(device_, "models/quad.obj");

			Object object;
			object.model = model;
			object.transform.translation = { 0.0f, 0.0f, 0.0f };
			object.transform.scale = Vec3f{ 3.0f, 1.0f, 3.0f };
			//object.transform.rotation.x = 0.1f * glm::two_pi<float>();

			objects_.push_back(std::move(object));
		}
	}
}


