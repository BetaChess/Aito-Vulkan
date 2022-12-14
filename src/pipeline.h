#ifndef AITO_PIPELINE_H
#define AITO_PIPELINE_H

#include "device.h"

#include <string>
#include <vector>

namespace aito
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline
	{
	public:
		// Constructor(s)
		Pipeline(
			Device& device, 
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo& configInfo);

		~Pipeline();

		// Not copyable or movable
		Pipeline(const Pipeline&) = delete;
		void operator=(const Pipeline&) = delete;
		Pipeline(Pipeline&&) = delete;
		Pipeline& operator=(Pipeline&&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		// Static methods
		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		// Private member variables
		Device& device_; // Pipeline has an aggregate relation to the device.
		VkPipeline graphicsPipeline_;
		VkShaderModule vertexShaderModule_;
		VkShaderModule fragmentShaderModule_;


		// Private methods
		static std::vector<char> readFile(const std::string& filename);

		void createGraphicsPipeline(
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	};

}


#endif /* AITO_PIPELINE_H */
