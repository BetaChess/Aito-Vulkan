#ifndef PHM_FRAME_INFO_H
#define PHM_FRAME_INFO_H

#include "camera.h"

#include <vulkan/vulkan.h>


namespace aito
{
	struct FrameInfo
	{
		int frameIndex;
		float deltaTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
}

#endif /* PHM_FRAME_INFO_H */
