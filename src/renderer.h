#ifndef AITO_RENDERER_H
#define AITO_RENDERER_H

#include <memory>
#include <vector>
#include <cassert>

#include "window.h"
#include "swapchain.h"


namespace aito
{
	class Renderer
	{
	public:
		Renderer(Window& window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		inline VkRenderPass getSwapChainRenderPass() const { return swapchain_->getRenderPass(); };
		inline bool isFrameInProgress() const { return isFrameStarted_; };
		inline float getAspectRatio() const { return swapchain_->extentAspectRatio(); };

		void populateImGui_initInfo(ImGui_ImplVulkan_InitInfo& init_info);

		inline VkCommandBuffer getCurrentCommandBuffer() const 
		{ 
			assert(isFrameStarted_ && "Tried to retrieve command buffer before a frame draw was initialised");
			return commandBuffers_[currentFrameIndex_]; 
		};

		inline int getFrameIndex() const 
		{ 
			assert(isFrameStarted_ && "Tried to retrieve frame index before a frame dra w was initialised");
			return currentFrameIndex_; 
		};

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		Window& window_; // The renderer has an aggregate relation to the window an device.
		Device& device_; // ^^^
		std::unique_ptr<Swapchain> swapchain_;
		std::vector<VkCommandBuffer> commandBuffers_;

		uint32_t currentImageIndex_ = 0;
		int currentFrameIndex_ = 0;
		bool isFrameStarted_ = false;


		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapchain();
	};
}

#endif /* AITO_RENDERER_H */