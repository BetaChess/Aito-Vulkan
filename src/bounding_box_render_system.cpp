#include "pch.h"

#include "bounding_box_render_system.h"
#include "time.h"

#include "vecmath.h"

#include <stdexcept>
#include <array>
#include <iostream>


namespace aito
{

std::vector<VkVertexInputBindingDescription> BoundingBox::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> BoundingBox::Vertex::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });	// position
	attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });		// color

	return attributeDescriptions;
}

//struct SimplePushConstantData
//{
//	Mat4f modelMatrix{ 1.0f };
//	Mat4f normalMatrix{ 1.0f };
//};

BoundingBoxRenderSystem::BoundingBoxRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	: device_(device)
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}

BoundingBoxRenderSystem::~BoundingBoxRenderSystem()
{
	vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}

// Self documenting
void BoundingBoxRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
	//VkPushConstantRange pushConstantRange{};
	//pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	//pushConstantRange.offset = 0;
	//pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr/*&pushConstantRange*/;

	if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout. ");
	}
}

void BoundingBoxRenderSystem::createPipeline(VkRenderPass renderPass)
{
	assert(
		pipelineLayout_ != nullptr &&
		"Cannot create pipeline before the pipeline layout"
	);

	aito::PipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	//pipelineConfig.rasterizationInfo.lineWidth = 3.0f;

	pipelineConfig.bindingDescriptions = BoundingBox::Vertex::getBindingDescriptions();
	pipelineConfig.attributeDescriptions = BoundingBox::Vertex::getAttributeDescriptions();

	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout_;

	pipeline_ = std::make_unique<Pipeline>(
		device_,
		"shaders/bounding_box.vert.spv",
		"shaders/bounding_box.frag.spv",
		pipelineConfig
		);

}

void BoundingBoxRenderSystem::renderObjects(
	const FrameInfo& frameInfo,
	const std::vector<BoundingBoxObject>& objects)
{
	pipeline_->bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(
		frameInfo.commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout_,
		0,
		1,
		&frameInfo.globalDescriptorSet,
		0,
		nullptr
	);

	for (auto& obj : objects)
	{
		/*SimplePushConstantData push{};
		push.modelMatrix = obj.transform.mat4();
		push.normalMatrix = obj.transform.normalMatrix();

		vkCmdPushConstants(
			frameInfo.commandBuffer,
			pipelineLayout_,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);*/
		obj.model->bind(frameInfo.commandBuffer);
		obj.model->draw(frameInfo.commandBuffer);
	}
}
BoundingBox::BoundingBox(Device& device, std::array<Vertex, 8> corners)
	: device_(device)
{
	createBuffers(corners);
}

BoundingBox::~BoundingBox()
{}

void BoundingBox::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer_->getBuffer() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void BoundingBox::draw(VkCommandBuffer commandBuffer)
{
	vkCmdDrawIndexed(commandBuffer, indexCount_, 1, 0, 0, 0);
}

void BoundingBox::createBuffers(const std::array<Vertex, 8>& vertices)
{
	// Create vertex buffer
	{
		const uint32_t vertexSize = sizeof(vertices[0]);

		// Create staging buffer for transfer to the GPU
		Buffer stagingBuffer{
			device_,
			vertexSize,
			vertexCount_,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Map the memory to the host and copy the data into the mapped region. 
		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertices.data());

		// Create the actual memory buffer on the GPU
		vertexBuffer_ = std::make_unique<Buffer>(
			device_,
			vertexSize,
			vertexCount_,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		// Copy the data from the staging buffer to the vertex buffer.
		device_.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer_->getBuffer(), stagingBuffer.getBufferSize());
	}
	// Create the index buffer
	{
		const uint32_t indexSize = sizeof(uint32_t);
		// Hardcode indices
		const std::array<uint32_t, 24> indices
		{
			0, 1, 0, 2, 2, 3, 3, 1,
			0, 4, 1, 5, 2, 6, 3, 7,
			4, 5, 4, 6, 6, 7, 5, 7
		};

		// Create staging buffer for transfer to the GPU
		Buffer stagingBuffer{
			device_,
			indexSize,
			indexCount_,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Map the memory to the host and copy the data into the mapped region.
		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)indices.data());

		// Create the actual memory buffer on the GPU
		indexBuffer_ = std::make_unique<Buffer>(
			device_,
			indexSize,
			indexCount_,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		// Copy the data from the staging buffer to the index buffer.
		device_.copyBuffer(stagingBuffer.getBuffer(), indexBuffer_->getBuffer(), stagingBuffer.getBufferSize());
	}
}

}

