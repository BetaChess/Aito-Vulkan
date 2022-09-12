#ifndef AITO_SHAPE_H
#define AITO_SHAPE_H

#include "device.h"
#include "buffer.h"

#include "vecmath.h"

#include <vector>
#include <memory>

namespace aito
{

// Something that can be rendered
class Model
{
public:

	// Public structs
	struct Vertex
	{
		Point3f position{};
		Vec3f color{};
		Vec3f normal{};
		glm::vec2 uv{};

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const;
	};

	// Temporary helper object for construction of models
	struct Builder
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		void loadModel(std::string_view filePath);
	};


	// Constructors

	Model(Device& device, const Model::Builder& builder);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	// Public methods

	static std::unique_ptr<Model> createModelFromFile(Device& device, std::string_view filePath);

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	Device& device_;

	std::unique_ptr<Buffer> vertexBuffer_;
	uint32_t vertexCount_;

	bool hasIndexBuffer = false;
	std::unique_ptr<Buffer> indexBuffer_;
	uint32_t indexCount_;

	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<uint32_t>& indices);
};

}


#endif // AITO_SHAPE_H