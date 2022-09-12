#ifndef AITO_TRANSFORM_H
#define AITO_TRANSFORM_H

#include <glm/gtc/matrix_transform.hpp>

namespace aito
{

struct Transform
{
	glm::vec3 translation{};
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };


	glm::mat4 mat4() const;
	glm::mat3 normalMatrix() const;

	glm::vec3 rotation{};
};

}


#endif // AITO_TRANSFORM_H