#ifndef AITO_BOUNDING_BOX_RENDER_SYSTEM_H
#define AITO_BOUNDING_BOX_RENDER_SYSTEM_H

#include <memory>
#include <vector>

#include "camera.h"
#include "pipeline.h"
#include "object.h"
#include "frame_info.h"
#include "bounds.h"


namespace aito
{
struct BoundingBox
{
	struct Vertex
	{
		inline Vertex()
		{}
		inline Vertex(const Point3f& pos)
			: position(pos)
		{};
		inline Vertex(const Point3f& pos, const Vec3f& col)
			: position(pos), color(col)
		{};
		inline Vertex(float x, float y, float z)
			: position(x, y, z)
		{};
		
		Point3f position{};
		Vec3f color{1.0f, 1.0f, 1.0f};

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	BoundingBox(Device& device, std::array<Vertex, 8> corners);
	template<typename T>
	BoundingBox(Device& device, Bounds3<T> bounds)
		: device_(device)
	{
		const std::array<BoundingBox::Vertex, 8> vertices =
		{
			bounds.corner(0),
			bounds.corner(1),
			bounds.corner(2),
			bounds.corner(3),
			bounds.corner(4),
			bounds.corner(5),
			bounds.corner(6),
			bounds.corner(7)
		};

		createBuffers(vertices);
	}
	template<typename T>
	BoundingBox(Device& device, Bounds3<T> bounds, Vec3f color)
		: device_(device)
	{
		const std::array<BoundingBox::Vertex, 8> vertices =
		{
			BoundingBox::Vertex(bounds.corner(0), color),
			BoundingBox::Vertex(bounds.corner(1), color),
			BoundingBox::Vertex(bounds.corner(2), color),
			BoundingBox::Vertex(bounds.corner(3), color),
			BoundingBox::Vertex(bounds.corner(4), color),
			BoundingBox::Vertex(bounds.corner(5), color),
			BoundingBox::Vertex(bounds.corner(6), color),
			BoundingBox::Vertex(bounds.corner(7), color)
		};

		createBuffers(vertices);
	}	
	~BoundingBox();

	BoundingBox(const BoundingBox&) = delete;
	BoundingBox& operator=(const BoundingBox&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	Device& device_;

	std::unique_ptr<Buffer> vertexBuffer_;
	static const uint32_t vertexCount_ = 8;

	bool hasIndexBuffer = false;
	std::unique_ptr<Buffer> indexBuffer_;
	static const uint32_t indexCount_ = 24;

	void createBuffers(const std::array<Vertex, 8>& vertices);
};

struct BoundingBoxObject
{
	std::shared_ptr<BoundingBox> model{};
	Transform transform{};
};

class BoundingBoxRenderSystem
{

public:
	BoundingBoxRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
	~BoundingBoxRenderSystem();

	BoundingBoxRenderSystem(const BoundingBoxRenderSystem&) = delete;
	BoundingBoxRenderSystem& operator=(const BoundingBoxRenderSystem&) = delete;

	void renderObjects(
		const FrameInfo& frameInfo,
		const std::vector<BoundingBoxObject>& objects);

private:
	Device& device_;

	std::unique_ptr<Pipeline> pipeline_;
	VkPipelineLayout pipelineLayout_;

	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass);
};

}

#endif /* AITO_BOUNDING_BOX_RENDER_SYSTEM_H */

