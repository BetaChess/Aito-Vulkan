#ifndef AITO_FRAME_INFO_H
#define AITO_FRAME_INFO_H

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

#endif /* AITO_FRAME_INFO_H */
