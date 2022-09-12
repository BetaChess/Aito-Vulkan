#ifndef PHM_POINT_LIGHT_SYSTEM_H
#define PHM_POINT_LIGHT_SYSTEM_H

#include <memory>
#include <vector>

#include "camera.h"
#include "pipeline.h"
#include "object.h"
#include "frame_info.h"


namespace aito
{
	class PointLightSystem
	{

	public:
		PointLightSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void renderObjects(const FrameInfo& frameInfo);

	private:
		Device& device_;

		std::unique_ptr<Pipeline> pipeline_;
		VkPipelineLayout pipelineLayout_;

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
}

#endif /* PHM_POINT_LIGHT_SYSTEM_H */

