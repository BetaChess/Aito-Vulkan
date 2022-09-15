#ifndef AITO_VECMATH_H
#define AITO_VECMATH_H

#include "aito.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>


namespace aito
{

typedef glm::vec<3, Float, glm::packed_highp> Vec3f;
typedef glm::vec<3, int, glm::packed_highp> Vec3i;
typedef glm::vec<4, Float, glm::packed_highp> Vec4f;
typedef glm::vec<4, int, glm::packed_highp> Vec4i;
typedef glm::vec<3, Float, glm::packed_highp> Point3f;
typedef glm::vec<3, int, glm::packed_highp> Point3i;
typedef glm::vec<4, Float, glm::packed_highp> Point4f;
typedef glm::mat<4, 4, Float, glm::packed_highp> Mat4f;
typedef glm::mat<3, 3, Float, glm::packed_highp> Mat3f;

template<typename T>
class Normal3 : public glm::vec<3, T, glm::packed_highp>
{
public:
	Normal3()
		: glm::vec<3, T, glm::packed_highp>()
	{};
	Normal3(const glm::vec<3, T, glm::packed_highp>& n)
		: glm::vec<3, T, glm::packed_highp>(n)
	{};
	Normal3(float x, float y, float z)
		: glm::vec<3, T, glm::packed_highp>(x, y, z)
	{};
};

}  // namespace aito



#endif // AITO_VECMATH_H